#pragma once

#include	<pybind11/pybind11.h>
#include	<pybind11/operators.h>
#include	"UniString.hpp"
#include	<pybind11/stl.h>

namespace pybind11 {
	namespace detail {

		template <> class type_caster<GS::UniString> {

		public:
			bool load(handle src, bool convert) {
				handle load_src = src;

				if (!src) {
					return false;
				}
				else if (!PyUnicode_Check(load_src.ptr()) && PYBIND11_BYTES_CHECK(load_src.ptr())) {
					const char *bytes = PYBIND11_BYTES_AS_STRING(src.ptr());
					if (bytes) {
						value = GS::UniString(bytes, (size_t)PYBIND11_BYTES_SIZE(src.ptr()));
						return true;
					}
				}

				object utfNbytes = reinterpret_steal<object>(PyUnicode_AsEncodedString(load_src.ptr(), "utf-8", nullptr));

				if (!utfNbytes) {
					PyErr_Clear();
					return false;
				}

				const char *buffer = reinterpret_cast<const char *>(PYBIND11_BYTES_AS_STRING(utfNbytes.ptr()));
				size_t length = (size_t)PYBIND11_BYTES_SIZE(utfNbytes.ptr()) / sizeof(char);

				value = GS::UniString(buffer, (GS::USize)length, CC_UTF8);

				return true;
			}

			static handle cast(GS::UniString src, return_value_policy /* policy */, handle /* parent */) {

				const char *buffer = reinterpret_cast<const char *>(src.ToCStr().Get());
				GS::Int32 nbytes = strlen(buffer);
				return PyUnicode_DecodeUTF8(buffer, nbytes, nullptr);
			}

			PYBIND11_TYPE_CASTER(GS::UniString, _(PYBIND11_STRING_NAME));
		};

		template <typename ArrayType, typename Value, bool Resizable, USize Size = 0> struct gs_array_caster {
			using value_conv = make_caster<Value>;

		private:
			template <bool R = Resizable>
			bool require_size(enable_if_t<R, USize> size) {
				if (value.GetSize() != size)
					value.EnsureSize(size);
				return true;
			}
			template <bool R = Resizable>
			bool require_size(enable_if_t<!R, USize> size) {
				return size == Size;
			}

		public:
			bool load(handle src, bool convert) {
				if (!isinstance<sequence>(src))
					return false;
				auto l = reinterpret_borrow<sequence>(src);
				if (!require_size(l.size()))
					return false;
				USize ctr = 0;
				for (auto it : l) {
					value_conv conv;
					if (!conv.load(it, convert))
						return false;
					value[ctr++] = cast_op<Value &&>(std::move(conv));
				}
				return true;
			}

			template <typename T>
			static handle cast(T &&src, return_value_policy policy, handle parent) {
				list l(src.GetSize());
				size_t index = 0;
				for (auto &&value : src) {
					auto value_ = reinterpret_steal<object>(value_conv::cast(forward_like<T>(value), policy, parent));
					if (!value_)
						return handle();
					PyList_SET_ITEM(l.ptr(), (ssize_t)index++, value_.release().ptr()); // steals a reference
				}
				return l.release();
			}

			PYBIND11_TYPE_CASTER(ArrayType, _("List[") + value_conv::name + _<Resizable>(_(""), _("[") + _<Size>() + _("]")) + _("]"));
		};

		template <typename Type> struct type_caster<GS::Array<Type>>
			: gs_array_caster<GS::Array<Type>, Type, true> {
		
		};

		template <> class type_caster<APP_Info> {

		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
					handle load_src = src;

					if (!src) {
						return false;
					}

					bool success = false;

					PyObject *exptObj = load_src.ptr();
					if (PyObject_HasAttrString(exptObj, "Dump")) {
						PyObject *dump_func = PyObject_GetAttrString(exptObj, "Dump");
						PyObject *dump_str = PyObject_CallObject(dump_func, NULL);

						if (PyErr_Occurred()) {
							PyErr_Print();
						}
						else {
							const char *bytes = PYBIND11_BYTES_AS_STRING(dump_str);
							if (bytes) {
								ssize_t dump_size = PYBIND11_BYTES_SIZE(dump_str);

								if (dump_size == sizeof(APP_Info)) {
									memcpy(&value, bytes, dump_size);
									
									if (value.iFunsSize == sizeof(ExportFuns)) {
										success = true;
									}
								}
							}
						}

						if (dump_str) {
							Py_DECREF(dump_str);
						}

						if (dump_func) {
							Py_DECREF(dump_func);
						}
					}

					if (!success) {
						throw cast_error("Please update ACExport.h in "
										  PROJECTNAME
										 " project to latest version; or update "
										  PROJECTNAME
										 " module.");
					}

					return success;
				}

			PYBIND11_NOINLINE static handle cast(APP_Info src, return_value_policy /* policy */, handle /* parent */) {
					return none().inc_ref();
				}

			PYBIND11_TYPE_CASTER(APP_Info, _("iTerm.APPInfo"));
		};

	}

	class gil_scoped_acquire_for_archicad {
		public:
			PYBIND11_NOINLINE gil_scoped_acquire_for_archicad(PyThreadState *tState) {
				this->m_state = tState;
				this->m_cur_state = PyThreadState_GET();

				if (this->m_cur_state&&this->m_cur_state->interp != this->m_state->interp) {
					PyEval_ReleaseThread(this->m_cur_state);
					this->m_old_state = PyThreadState_Swap(NULL);
					PyEval_AcquireThread(this->m_state);
					this->m_sav_state = PyThreadState_Swap(this->m_state);
				}
				else if (this->m_cur_state&&this->m_cur_state == this->m_state) {

				}
				else {
					PyEval_RestoreThread(this->m_state);
				}
			}

			PYBIND11_NOINLINE ~gil_scoped_acquire_for_archicad() {
				if (this->m_cur_state&&this->m_cur_state->interp != this->m_state->interp) {
					PyThreadState_Swap(this->m_sav_state);
					PyEval_ReleaseThread(this->m_state);
					PyThreadState_Swap(this->m_old_state);
					PyEval_AcquireThread(this->m_cur_state);
				}
				else if (this->m_cur_state&&this->m_cur_state == this->m_state) {

				}
				else {
					PyEval_SaveThread();
				}
			}
		private:
			PyThreadState	*m_cur_state;
			PyThreadState	*m_sav_state;
			PyThreadState	*m_old_state;
			PyThreadState	*m_state;
	};

}

namespace py = pybind11;

PyThreadState *GetCurrentAppState() {
	py::module m_sys = py::module::import("sys");
	py::detail::type_caster<APP_Info> caster;
	if (caster.load(m_sys.attr("vAppInfo"),true)) {
		return (*caster).iState;
	}
	throw py::type_error();
}

ExportFuns *GetExtFuncs() {
	py::module m_sys = py::module::import("sys");
	py::detail::type_caster<APP_Info> caster;
	if (caster.load(m_sys.attr("vAppInfo"),true)) {
		return (*caster).iFuns;
	}
	throw py::type_error();
}