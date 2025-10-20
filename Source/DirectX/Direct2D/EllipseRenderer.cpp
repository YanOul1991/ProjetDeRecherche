#include "DirectX/Direct2D/EllipseRenderer.h"
#include "DirectX/Direct2D/Renderer.h"

EllipseRenderer::EllipseRenderer( float _position_x, float _position_y, float _radius) : 
  position_x  { _position_x },
  position_y  { _position_y },
  radius      { _radius }
{
  m_ellipse.point = D2D1_POINT_2F(position_x, position_y);
  m_ellipse.radiusX = radius;
  m_ellipse.radiusY = radius;
}

EllipseRenderer::EllipseRenderer()
{ }

void EllipseRenderer::update()
{
  radius += 0.02f;
  m_ellipse.point = D2D1::Point2F(200, 200);
  m_ellipse.radiusX = radius;
  m_ellipse.radiusY = radius;
}