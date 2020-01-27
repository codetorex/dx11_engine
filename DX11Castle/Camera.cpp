#include "stdafx.h"
#include "Camera.h"
#include "Engine.h"

void CameraTPSController::Update()
{

	HWND k = GetForegroundWindow();
	if ( k != Engine.m_Renderer.m_Window.m_Hwnd )
	{
		return;
	}

	XMVECTOR direction = m_Cam.Focus - m_Cam.Eye;
	XMVECTOR magnitude = XMVector3Length( direction );
	direction = XMVector3Normalize( direction );

	XMVECTOR upnorm = XMVector3Normalize( m_Cam.Up );
	XMVECTOR right = XMVector3Cross( direction, upnorm );

	XMVECTOR forewardVec = direction * (m_MoveSpeed * Engine.m_Time.m_Diff);

	if ( m_Forward )
	{
		m_Cam.Eye += forewardVec;
		m_Cam.Focus += forewardVec;
	}
	else if ( m_Backward )
	{
		m_Cam.Eye -= forewardVec;
		m_Cam.Focus -= forewardVec;
	}

	XMVECTOR strafeVec = -right * (m_MoveSpeed * Engine.m_Time.m_Diff);
	if ( m_StrafeRight )
	{
		m_Cam.Eye += strafeVec;
		m_Cam.Focus += strafeVec;
	}
	else if ( m_StrafeLeft )
	{
		m_Cam.Eye -= strafeVec;
		m_Cam.Focus -= strafeVec;
	}

	POINT p;
	GetCursorPos( &p );

	m_MouseDiffX = p.x - 500;
	m_MouseDiffY = p.y - 500;


	SetCursorPos( 500, 500 );




	if ( m_MouseDiffY != 0 )
	{
		XMVECTOR direction = m_Cam.Focus - m_Cam.Eye;
		XMVECTOR magnitude = XMVector3Length( direction ); 
		direction = XMVector3Normalize( direction );
		XMVECTOR upnorm = XMVector3Normalize( m_Cam.Up );
		XMVECTOR right = XMVector3Cross( direction, upnorm );

		XMMATRIX rotation = XMMatrixRotationNormal( -right, XMConvertToRadians( (float)m_MouseDiffY * m_RotationSpeed ) );

		m_Cam.Focus = (XMVector3Transform( direction, rotation ) * magnitude) + m_Cam.Eye;
		m_Cam.Up = XMVector3Transform( m_Cam.Up, rotation );

	}

	if ( m_MouseDiffX != 0 )
	{
		XMVECTOR direction = m_Cam.Focus - m_Cam.Eye;
		XMVECTOR magnitude = XMVector3Length( direction ); 
		direction = XMVector3Normalize( direction );
		XMVECTOR upnorm = XMVector3Normalize( m_Cam.Up );
		XMVECTOR right = XMVector3Cross( direction, upnorm );

		XMMATRIX rotation = XMMatrixRotationNormal( XMVectorSet( 0.0f, 1.0f, 0.0f, 0 ),
													XMConvertToRadians( (float)m_MouseDiffX * m_RotationSpeed ) );

		m_Cam.Focus = (XMVector3Transform( direction, rotation ) * magnitude) + m_Cam.Eye;
		m_Cam.Up = XMVector3Transform( m_Cam.Up, rotation );
	}

	m_Cam.Update();
}
