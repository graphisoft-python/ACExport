#pragma once

#include	<pybind11/pybind11.h>

#include	"Point.hpp"
#include	"DGUtility.hpp"
//#include "Rect.hpp"

namespace adapter {
	class Point {
	public:
		Point() :
			x(0), y(0), scale(1) {

		}

		Point(const double &x, const double &y) :
			x(x), y(y), scale(1) {

		}

		Point(const double &x, const double &y, const double& scale) :
			x(x), y(y), scale(scale) {

		}

		bool	operator== (const Point& rightOp) const {
			return (this->x == rightOp.x&&this->y == rightOp.y);
		}
		bool	operator!= (const Point& rightOp) const {
			return (this->x != rightOp.x || this->y != rightOp.y);
		}

		void	Set(const double& x, const double& y) {
			this->x = x;
			this->y = y;
		}

		void	SetX(const double& x) {
			this->x = x;
		}

		void	SetY(const double& y) {
			this->y = y;
		}

		double	GetX(void) const {
			return this->x;
		}

		double	GetY(void) const {
			return this->y;
		}

		double	GetScale(void) const {
			return this->scale;
		}

		void Offset(const double &x, const double &y) {
			this->x += x;
			this->y += y;
		}

	private:
		double x;
		double y;
		double scale;
	};
}


namespace pybind11 {
	namespace detail {
		template <> class type_caster<GS::Point<float>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");
				
				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = GS::Point<float>(static_cast<float>(x), static_cast<float>(y));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(GS::Point<float> src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");
				
				double x = static_cast<double>(src.GetX());
				double y = static_cast<double>(src.GetY());

				return GSPointType(
					x,y
				).release();
			}

			PYBIND11_TYPE_CASTER(GS::Point<float>, _("GSRoot.Point"));
		};

		template <> class type_caster<GS::Point<short>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");

				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = GS::Point<short>(static_cast<short>(x), static_cast<short>(y));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(GS::Point<short> src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");

				double x = static_cast<double>(src.GetX());
				double y = static_cast<double>(src.GetY());

				return GSPointType(
					x, y
				).release();
			}

			PYBIND11_TYPE_CASTER(GS::Point<short>, _("GSRoot.Point"));
		};

		template <> class type_caster<GS::Point<DG::NativeUnit>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");

				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = GS::Point<DG::NativeUnit>(DG::NativeUnit(static_cast<short>(x),scale), DG::NativeUnit(static_cast<short>(y), scale));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(GS::Point<DG::NativeUnit> src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");

				double x = static_cast<double>(src.GetX().GetValue());
				double y = static_cast<double>(src.GetY().GetValue());

				return GSPointType(
					x, y
				).release();
			}

			PYBIND11_TYPE_CASTER(GS::Point<DG::NativeUnit>, _("GSRoot.Point"));
		};

		template <> class type_caster<DG::TPoint<short>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");

				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::TPoint<short>(static_cast<short>(x), static_cast<short>(y));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::TPoint<short> src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");

				double x = static_cast<double>(src.GetX());
				double y = static_cast<double>(src.GetY());

				return GSPointType(
					x, y
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::TPoint<short>, _("GSRoot.Point"));
		};

		template <> class type_caster<DG::TPoint<DG::NativeUnit>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");

				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::TPoint<DG::NativeUnit>(DG::NativeUnit(static_cast<short>(x),scale), DG::NativeUnit(static_cast<short>(y),scale));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::TPoint<DG::NativeUnit> src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");

				double x = static_cast<double>(src.GetX().GetValue());
				double y = static_cast<double>(src.GetY().GetValue());

				return GSPointType(
					x, y
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::TPoint<DG::NativeUnit>, _("GSRoot.Point"));
		};

		template <> class type_caster<DG::Point> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");

				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::Point(static_cast<short>(x), static_cast<short>(y));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::Point src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");

				double x = static_cast<double>(src.GetX());
				double y = static_cast<double>(src.GetY());

				return GSPointType(
					x, y
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::Point, _("GSRoot.Point"));
		};
	
		template <> class type_caster<DG::NativePoint> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSPointType = module::import("GSRoot").attr("Point");

				if (!obj.get_type().is(GSPointType)) {
					return false;
				}

				double x = obj.attr("GetX")().cast<double>();
				double y = obj.attr("GetY")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::NativePoint(DG::NativeUnit(static_cast<short>(x), scale), DG::NativeUnit(static_cast<short>(y), scale));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::NativePoint src, return_value_policy /* policy */, handle /* parent */) {
				object GSPointType = module::import("GSRoot").attr("Point");

				double x = static_cast<double>(src.GetX().GetValue());
				double y = static_cast<double>(src.GetY().GetValue());

				return GSPointType(
					x, y
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::NativePoint, _("GSRoot.Point"));
		};
	}
}

