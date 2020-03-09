#pragma once

#include	<pybind11/pybind11.h>
#include "BaseAlgorithms.hpp"

#include "Rect.hpp"
#include "DGUtility.hpp"
#include "gs_point.hpp"



namespace adapter {
	class  Rect {
	public:
		Rect() :
			left(0), top(0), right(0), bottom(0),scale(1) {

		}

		Rect(const double& newLeft, const double& newTop, const double& newRight, const double& newBottom) :
			left(newLeft),top(newTop),right(newRight),bottom(newBottom),scale(1){
			this->Normalize();
		}

		Rect(const double& newLeft, const double& newTop, const double& newRight, const double& newBottom,const double&newScale) :
			left(newLeft), top(newTop), right(newRight), bottom(newBottom),scale(newScale) {
			this->Normalize();
		}

		Rect(const Point& p) :
			left(p.GetX()),top(p.GetY()),right(p.GetX()),bottom(p.GetY()),scale(1) {
			this->Normalize();
		}

		Rect(const Point& p,const double &newScale) :
			left(p.GetX()), top(p.GetY()), right(p.GetX()), bottom(p.GetY()),scale(newScale){
			this->Normalize();
		}

		void	Reset(void) {
			this->left = 0;
			this->top = 0;
			this->right = 0;
			this->bottom = 0;
		}
		void	Set(const double& newLeft, const double& newTop, const double& newRight, const double& newBottom) {
			this->left = newLeft;
			this->top = newTop;
			this->right = newRight;
			this->bottom = newBottom;

			this->Normalize();
		}
		void	Set(const Point& p) {
			this->left = p.GetX();
			this->top = p.GetY();
			this->right = this->left;
			this->bottom = this->top;

			this->Normalize();
		}
		void	SetWithSize(const double& newLeft, const double& newTop, const double& height, const double& width) {
			this->left = newLeft;
			this->top = newTop;
			this->right = static_cast<double>(this->left + width);
			this->bottom = static_cast<double>(this->top + height);

			this->Normalize();
		}
		double	GetLeft(void) const {
			return this->left;
		}
		double	GetTop(void) const {
			return this->top;
		}
		double	GetRight(void) const {
			return this->right;
		}
		double	GetBottom(void) const {
			return this->bottom;
		}
		double	GetWidth(void) const {
			return this->right - this->left;
		}
		double	GetHeight(void) const {
			return this->bottom - this->top;
		}
		void	SetLeft(const double& newLeft) {
			this->left = newLeft;
			this->Normalize();
		}
		void	SetTop(const double& newTop) {
			this->top = newTop;
			this->Normalize();
		}
		void	SetRight(const double& newRight) {
			this->right = newRight;
			this->Normalize();
		}
		void	SetBottom(const double& newBottom) {
			this->bottom = newBottom;
			this->Normalize();
		}
		void	SetWidth(const double& newWidth) {
			this->right = this->left + newWidth;
			this->Normalize();
		}
		void	SetHeight(const double& newHeight) {
			this->bottom = this->top + newHeight;
			this->Normalize();
		}
		void	SetSize(const double& newWidth, const double& newHeight) {
			this->SetWidth(newWidth);
			this->SetHeight(newHeight);
		}
		void	Resize(const double& deltaW, const double& deltaH) {
			this->SetWidth(this->GetWidth() + deltaW);
			this->SetHeight(this->GetHeight() + deltaH);
		}
		void	Inset(const double& deltaX, const double& deltaY) {
			this->left = this->left + deltaX;
			this->top = this->top + deltaY;
			this->right = this->right - deltaX;
			this->bottom = this->bottom - deltaY;
			this->Normalize();
		}
		void	Offset(const double& deltaX, const double& deltaY) {
			this->left = this->left + deltaX;
			this->top = this->top + deltaY;
			this->right = this->right + deltaX;
			this->bottom = this->bottom + deltaY;
		}

		bool	operator== (const Rect& other) const {
			return this->left == other.left&&
				this->top == other.top&&
				this->right == other.right&&
				this->bottom == other.bottom;
		}
		bool	operator!= (const Rect& other) const {
			return this->left != other.left ||
				this->top != other.top ||
				this->right != other.right ||
				this->bottom != other.bottom;
		}
		inline  bool	IsOverlapping(const Rect& other) const {
			return (GS::Max(this->left, other.left) <= GS::Min(this->right, other.right) && GS::Max(this->top, other.top) <= GS::Min(this->bottom, other.bottom));
		}
		bool	IsInside(const double& xPosition, const double& yPosition) const {
			return (GS::IsBetween(xPosition, left, right) &&
				GS::IsBetween(yPosition, top, bottom));
		}
		bool	IsInside(const Point& p) const {
			return (GS::IsBetween(p.GetX(), left, right) &&
				GS::IsBetween(p.GetY(), top, bottom));
		}
		bool	IsInside(const Rect& other) const {
			return (other.left >= left && other.right <= right &&
				other.top >= top && other.bottom <= bottom);
		}
		bool	IsOutside(const Rect& other) const {
			return (other.left > right || other.right < left ||
				other.top > bottom || other.bottom < top);
		}
		bool	IsEmpty(void) const {
			return (left == right && top == bottom);
		}

		Rect	Union(const Rect& other) const {
			Rect t(other);

			if (left != 0 || top != 0 || right != 0 || bottom != 0) {
				t.left = GS::Min(left, t.left);
				t.top = GS::Min(top, t.top);
				t.right = GS::Max(right, t.right);
				t.bottom = GS::Max(bottom, t.bottom);
			}

			return t;
		}
		Rect	Union(const Point& p) const {
			Rect toUnion(p);
			Rect t(toUnion);

			if (left != 0 || top != 0 || right != 0 || bottom != 0) {
				t.left = GS::Min(left, t.left);
				t.top = GS::Min(top, t.top);
				t.right = GS::Max(right, t.right);
				t.bottom = GS::Max(bottom, t.bottom);
			}

			return t;
		}
		Rect	Intersection(const Rect& other) const {
			if (!IsOverlapping(other))
				return Rect();

			Rect t(other);

			if (left != 0 || top != 0 || right != 0 || bottom != 0) {
				t.left = GS::Max(left, t.left);
				t.top = GS::Max(top, t.top);
				t.right = GS::Min(right, t.right);
				t.bottom = GS::Min(bottom, t.bottom);
			}
			else {
				t.Reset();
			}

			return t;
		}

		void	Unify(const Rect& other) {
			if (left != 0 || top != 0 || right != 0 || bottom != 0) {
				left = GS::Min(left, other.left);
				top = GS::Min(top, other.top);
				right = GS::Max(right, other.right);
				bottom = GS::Max(bottom, other.bottom);
			}
			else {
				Set(other.left, other.top, other.right, other.bottom);
			}
		}
		void	Unify(const Point& p) {
			if (left !=0 || top != 0 || right != 0 || bottom != 0) {	// TZs SPEED: 16007
				left = GS::Min(left, p.GetX());
				top = GS::Min(top, p.GetY());
				right = GS::Max(right, p.GetX());
				bottom = GS::Max(bottom, p.GetY());
			}
			else {
				Set(p.GetX(), p.GetY(), p.GetX(), p.GetY());
			}
		}
		void	Intersect(const Rect& other) {
			if (!IsOverlapping(other)) {
				*this = Rect ();
			}

			if (left != 0 || top != 0 || right != 0 || bottom != 0) {
				left = GS::Max(left, other.left);
				top = GS::Max(top, other.top);
				right = GS::Min(right, other.right);
				bottom = GS::Min(bottom, other.bottom);
			}
			else {
				Reset();
			}
		}


	public:
		Rect(const Point& pt1, const Point& pt2) :
			left(pt1.GetX()),top(pt1.GetY()),right(pt2.GetX()),bottom(pt2.GetY()),scale(1){
			this->Normalize();
		}
		Rect(const Point& pt1, const Point& pt2, const double &newScale):
			left(pt1.GetX()), top(pt1.GetY()), right(pt2.GetX()), bottom(pt2.GetY()), scale(newScale) {
			this->Normalize();
		}
		Rect(const Point& pt, const double& width, const double& height) :
			left(pt.GetX()),top(pt.GetY()),right(left+width),bottom(top+height),scale(1){
			this->Normalize();
		}
		Rect(const Point& pt, const double& width, const double& height, const double& newScale) :
			left(pt.GetX()), top(pt.GetY()), right(left + width), bottom(top + height), scale(newScale) {
			this->Normalize();
		}

		void	Set(const Rect& sourceRect) {
			this->left = sourceRect.left;
			this->top = sourceRect.top;
			this->right = sourceRect.right;
			this->bottom = sourceRect.bottom;
			this->scale = sourceRect.scale;

			this->Normalize();
		}
		void	Set(const Point& pt1, const Point& pt2) {
			this->left = pt1.GetX();
			this->top = pt1.GetY();
			this->right = pt2.GetX();
			this->bottom = pt2.GetY();

			this->Normalize();
		}
		void	Set(const Point& pt, const double& width, const double& height) {
			this->left = pt.GetX();
			this->top = pt.GetY();
			this->right = this->left + width;
			this->bottom = this->bottom + height;

			this->Normalize();
		}


		void	SetLeftTop(const Point& pt) {
			this->left = pt.GetX();
			this->top = pt.GetY();

			this->Normalize();
		}
		Point	GetLeftTop(void) const {
			return Point(this->left, this->top);
		}

		void	SetLeftBottom(const Point& pt) {
			this->left = pt.GetX();
			this->bottom = pt.GetY();
			this->Normalize();
		}
		Point	GetLeftBottom(void) const {
			return Point(this->left, this->bottom);
		}

		void	SetRightTop(const Point& pt) {
			this->right = pt.GetX();
			this->top = pt.GetY();

			this->Normalize();
		}
		Point	GetRightTop(void) const {
			return Point(this->right, this->top);
		}

		void	SetRightBottom(const Point& pt) {
			this->right = pt.GetX();
			this->bottom = pt.GetY();
			this->Normalize();
		}
		Point	GetRightBottom(void) const {
			return Point(this->right, this->bottom);
		}

		void	SetCenter(Point& pt) {
			double width = right - left;
			left = pt.GetX() - width / 2;
			right = left + width;

			double height = bottom - top;
			top = pt.GetY() - height / 2;
			bottom = top + height;

			this->Normalize();
		}
		Point	GetCenter(void) const {
			return Point((left + right) / 2, (top + bottom) / 2);
		}


		void	Move(const double& dx, const double& dy) {
			this->Offset(dx, dy);
		}
		void	Inflate(const double& dx, const double& dy) {
			this->left = this->left - dx;
			this->right = this->right + dx;
			this->top = this->top - dy;
			this->bottom = this->bottom + dy;

			this->Normalize();
		}

		bool	Contains(const Point& pt) const {
			return left <= pt.GetX() && right >= pt.GetX() && top <= pt.GetY() && bottom >= pt.GetY();
		}
		bool	Contains(const double& x, const double& y) const {
			return left <= x && right >= x && top <= y && bottom >= y;
		}

		bool		IsIntersecting(const Rect& otherRect) const {
			bool horizontalExcluded = left > otherRect.right || right < otherRect.left;
			bool verticalExcluded = top > otherRect.bottom || bottom < otherRect.top;

			return !horizontalExcluded && !verticalExcluded;
		}
		//TRect<TYPE>	Intersect(const TRect<TYPE>& otherRect) const;

		double GetScale()const {
			return this->scale;
		}
	private:
		double left;
		double top;
		double right;
		double bottom;
		double scale;

		void Normalize() {
			if (this->top > this->bottom) {
				GS::Swap(this->top, this->bottom);
			}

			if (this->left > this->right) {
				GS::Swap(this->left, this->right);
			}
		}
	};
}

namespace pybind11 {
	namespace detail {
		template <> class type_caster<GS::Rect<float>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right= obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = GS::Rect<float>(static_cast<float>(left),
					static_cast<float>(top),
					static_cast<float>(right),
					static_cast<float>(bottom));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(GS::Rect<float> src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft());
				double top = static_cast<double>(src.GetTop());
				double right = static_cast<double>(src.GetRight());
				double bottom = static_cast<double>(src.GetBottom());
				double scale = 1;


				return GSRectType(
					left,top,right,bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(GS::Rect<float>, _("GSRoot.Rect"));
		};

		template <> class type_caster<GS::Rect<short>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right = obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = GS::Rect<short>(static_cast<short>(left),
					static_cast<short>(top),
					static_cast<short>(right),
					static_cast<short>(bottom));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(GS::Rect<short> src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft());
				double top = static_cast<double>(src.GetTop());
				double right = static_cast<double>(src.GetRight());
				double bottom = static_cast<double>(src.GetBottom());
				double scale = 1;


				return GSRectType(
					left, top, right, bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(GS::Rect<short>, _("GSRoot.Rect"));
		};

		template <> class type_caster<GS::Rect<DG::NativeUnit>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right = obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = GS::Rect< DG::NativeUnit>(DG::NativeUnit(static_cast<short>(left), scale),
					DG::NativeUnit(static_cast<short>(top), scale),
					DG::NativeUnit(static_cast<short>(right), scale),
					DG::NativeUnit(static_cast<short>(bottom), scale));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(GS::Rect< DG::NativeUnit> src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft().GetValue());
				double top = static_cast<double>(src.GetTop().GetValue());
				double right = static_cast<double>(src.GetRight().GetValue());
				double bottom = static_cast<double>(src.GetBottom().GetValue());
				double scale = 1;


				return GSRectType(
					left, top, right, bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(GS::Rect<DG::NativeUnit>, _("GSRoot.Rect"));
		};

		template <> class type_caster<DG::TRect<short>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right = obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::TRect<short>(static_cast<short>(left),
					static_cast<short>(top),
					static_cast<short>(right),
					static_cast<short>(bottom));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::TRect<short> src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft());
				double top = static_cast<double>(src.GetTop());
				double right = static_cast<double>(src.GetRight());
				double bottom = static_cast<double>(src.GetBottom());
				double scale = 1;


				return GSRectType(
					left, top, right, bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::TRect<short>, _("GSRoot.Rect"));
		};

		template <> class type_caster<DG::TRect<DG::NativeUnit>> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right = obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::TRect< DG::NativeUnit>(DG::NativeUnit(static_cast<short>(left), scale),
					DG::NativeUnit(static_cast<short>(top), scale),
					DG::NativeUnit(static_cast<short>(right), scale),
					DG::NativeUnit(static_cast<short>(bottom), scale));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::TRect< DG::NativeUnit> src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft().GetValue());
				double top = static_cast<double>(src.GetTop().GetValue());
				double right = static_cast<double>(src.GetRight().GetValue());
				double bottom = static_cast<double>(src.GetBottom().GetValue());
				double scale = 1;


				return GSRectType(
					left, top, right, bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::TRect<DG::NativeUnit>, _("GSRoot.Rect"));
		};

		template <> class type_caster<DG::Rect> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right = obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::Rect(static_cast<short>(left),
					static_cast<short>(top),
					static_cast<short>(right),
					static_cast<short>(bottom));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::Rect src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft());
				double top = static_cast<double>(src.GetTop());
				double right = static_cast<double>(src.GetRight());
				double bottom = static_cast<double>(src.GetBottom());
				double scale = 1;


				return GSRectType(
					left, top, right, bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::Rect, _("GSRoot.Rect"));
		};
	
		template <> class type_caster<DG::NativeRect> {
		public:
			PYBIND11_NOINLINE bool load(handle src, bool convert) {
				handle load_src = src;
				if (!src) {
					return false;
				}

				auto obj = reinterpret_borrow<object>(src);
				object GSRectType = module::import("GSRoot").attr("Rect");

				if (!obj.get_type().is(GSRectType)) {
					return false;
				}

				double left = obj.attr("GetLeft")().cast<double>();
				double top = obj.attr("GetTop")().cast<double>();
				double right = obj.attr("GetRight")().cast<double>();
				double bottom = obj.attr("GetBottom")().cast<double>();
				double scale = obj.attr("GetScale")().cast<double>();

				this->value = DG::NativeRect(DG::NativeUnit(static_cast<short>(left), scale),
					DG::NativeUnit(static_cast<short>(top), scale),
					DG::NativeUnit(static_cast<short>(right), scale),
					DG::NativeUnit(static_cast<short>(bottom), scale));

				return true;
			}

			PYBIND11_NOINLINE static handle cast(DG::NativeRect src, return_value_policy /* policy */, handle /* parent */) {
				object GSRectType = module::import("GSRoot").attr("Rect");

				double left = static_cast<double>(src.GetLeft().GetValue());
				double top = static_cast<double>(src.GetTop().GetValue());
				double right = static_cast<double>(src.GetRight().GetValue());
				double bottom = static_cast<double>(src.GetBottom().GetValue());
				double scale = 1;


				return GSRectType(
					left, top, right, bottom
				).release();
			}

			PYBIND11_TYPE_CASTER(DG::NativeRect, _("GSRoot.Rect"));
		};
	}
}