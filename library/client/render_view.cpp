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

Bounds Bounds::center( const Bounds& outter, const Vector& rect )
{
	return Bounds(outter.position + (outter.size / 2 - rect / 2), rect);
}







RenderView::RenderView( sf::RenderTarget& target, const Rect& tileSize )
	: mTarget(target)
	, mOldView(target.getView())
	, mSize(target.getSize().x / float(tileSize.w), target.getSize().y / float(tileSize.h))
{
}

RenderView::RenderView( RenderView& view, const Bounds& outter_bounds)
	: mTarget(view.target())
	, mSize(outter_bounds.size)
	, mOldView(mTarget.getView())
{
	sf::View newView(sf::FloatRect(0.0f, 0.0f, outter_bounds.size.w, outter_bounds.size.h));
	newView.setViewport(_calcViewport(outter_bounds, view.mSize));

	mTarget.setView(newView);
}

RenderView::RenderView( RenderView& view, const Bounds& outter_bounds, const Bounds& inner_bounds )
	: mTarget(view.target())
	, mSize(inner_bounds.size)
	, mOldView(mTarget.getView())
{
	sf::View newView(sf::FloatRect(inner_bounds.position.x, inner_bounds.position.y, inner_bounds.size.w, inner_bounds.size.h));
	newView.setViewport(_calcViewport(outter_bounds, view.mSize));

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

}