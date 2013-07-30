#include "includes.hpp"
#include "render_view.hpp"


namespace client {






Bounds::Bounds()
{

}

Bounds::Bounds( const Vector& pos, const Vector& size )
	: position(pos)
	, size(size)
{
}

Bounds::~Bounds()
{
}

Bounds Bounds::center( const Bounds& outter, const Vector& rect)
{
	return Bounds(outter.position + (outter.size / 2 - rect / 2), rect);
}

client::Bounds Bounds::applyAspect( const Aspect& aspect ) const
{
	return Bounds(position * aspect, size *aspect);
}

client::Bounds Bounds::removeAspect( const Aspect& aspect ) const
{
	return Bounds(position / aspect, size / aspect);
}









RenderView::RenderView( sf::RenderTarget& target)
	: mTarget(target)
	, mOldView(target.getView())
	, mSize(target.getSize().x, target.getSize().y)
	, mColor(sf::Color::White)
{
}

RenderView::RenderView( RenderView& view, const Bounds& outter_bounds, const Aspect& aspect)
	: mTarget(view.target())
	, mSize(outter_bounds.size * static_cast<Vector>(aspect))
	, mOldView(mTarget.getView())
	, mColor(view.color())
{
	sf::View newView(sf::FloatRect(0.0f, 0.0f, outter_bounds.size.w, outter_bounds.size.h));
	newView.setViewport(_calcViewport(outter_bounds.applyAspect(aspect), view.mSize));

	mTarget.setView(newView);
}

RenderView::RenderView(RenderView& view, const Bounds& outter_bounds, const Aspect& outter_aspect, const Bounds& inner_bounds, const Aspect& inner_aspect, const sf::Color& color)
	: mTarget(view.target())
	, mSize(inner_bounds.size * static_cast<Vector>(inner_aspect))
	, mOldView(mTarget.getView())
	, mColor(color * view.color())
{
	sf::View newView(sf::FloatRect(inner_bounds.position.x, inner_bounds.position.y, inner_bounds.size.w, inner_bounds.size.h));
	newView.setViewport(_calcViewport(outter_bounds.applyAspect(outter_aspect), view.mSize));

	mTarget.setView(newView);
}

RenderView::~RenderView()
{
	mTarget.setView(mOldView);
}

sf::RenderTarget& RenderView::target()
{
	return mTarget;
}

Bounds RenderView::bounds() const
{
	return Bounds(Vector(), mSize);
}

sf::FloatRect RenderView::_calcViewport(const Bounds& outter, const Vector& topSize)
{
	auto& old_vp = mOldView.getViewport();
	Vector old_pos(old_vp.left, old_vp.top);
	Vector old_size(old_vp.width, old_vp.height);

	Vector rpos = ((outter.position * old_size) / topSize) + old_pos;
	Vector rsize = (outter.size * old_size) / topSize;

	return sf::FloatRect(rpos.x, rpos.y, rsize.w, rsize.h);
}

const sf::Color& RenderView::color() const
{
	return mColor;
}

}