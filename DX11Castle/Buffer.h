#ifndef BUFFER_H
#define BUFFER_H

#include "Interfaces.h"
#include "Engine.h"

class Buffer : public IResource
{
public:
	ID3D11Buffer *m_Buffer;

	Buffer()
	{
		m_Buffer = NULL;
	}

	void Release()
	{
		m_Buffer->Release();
	}
};

template<class T>
class ConstantBuffer : public Buffer, public IUpdateable
{
public:
	T m_State;

	ConstantBuffer()
	{
		Create();
	}

	void Update()
	{
		Engine.m_Renderer.m_Context->UpdateSubresource( m_Buffer, 0, NULL, &m_State, 0, 0 );
	}

	ConstantBuffer& operator = (T& value)
	{
		m_State = value;
		Update();
		return *this;
	}

	void Create()
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = Engine.m_Renderer.m_Device->CreateBuffer( &bd, NULL, &m_Buffer );
		if ( FAILED( hr ) )
			throw exception( "create buffer failed" );
	}
};

template<class T, int Slot>
class PSConstantBuffer : public ConstantBuffer<T>, public IRenderable
{
public:
	PSConstantBuffer& operator = (T& value)
	{
		m_State = value;
		Update();
		return *this;
	}

	inline void Render()
	{
		Engine.m_Renderer.m_Context->PSSetConstantBuffers( Slot, 1, &m_Buffer );
	}
};

template<class T, int Slot>
class GSConstantBuffer : public ConstantBuffer<T>, public IRenderable
{
public:
	GSConstantBuffer& operator = (T& value)
	{
		m_State = value;
		Update();
		return *this;
	}

	inline void Render()
	{
		Engine.m_Renderer.m_Context->GSSetConstantBuffers( Slot, 1, &m_Buffer );
	}
};

template<class T, int Slot>
class VSConstantBuffer : public ConstantBuffer<T>, public IRenderable
{
public:
	VSConstantBuffer& operator = (T& value)
	{
		m_State = value;
		Update();
		return *this;
	}

	inline void Render()
	{
		Engine.m_Renderer.m_Context->VSSetConstantBuffers( Slot, 1, &m_Buffer );
	}
};


class IndexBuffer : public Buffer, public IRenderable
{
public:
	int m_IndexCount;
	int m_FaceCount;
	std::vector<UINT16> m_Indexes;

	void Reset()
	{
		m_Indexes.clear();
		m_IndexCount = 0;
		m_Buffer->Release();
		m_Buffer = NULL;
	}

	inline void AddIndex( UINT16 i )
	{
		m_Indexes.push_back( i );
		m_IndexCount = m_Indexes.size();
	}

	void Update()
	{
		Engine.m_Renderer.m_Context->UpdateSubresource( m_Buffer, 0, NULL, m_Indexes.data(), 0, 0 );
	}

	void Render()
	{
		// Set index buffer
		UINT stride = sizeof(UINT16);
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetIndexBuffer( m_Buffer, DXGI_FORMAT_R16_UINT, offset );

		Engine.m_Renderer.m_Context->DrawIndexed( m_IndexCount, 0, 0 );
	}

	void Create()
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(UINT16)* m_IndexCount;	// from sortOfMesh
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = m_Indexes.data();	// from sortOfMesh	

		HRESULT hr = Engine.m_Renderer.m_Device->CreateBuffer( &bd, &InitData, &m_Buffer );
		if ( FAILED( hr ) )
			throw exception( "Create index buffer failed" );
	}

	void Clear()
	{
		m_IndexCount = 0;
		m_Indexes.clear();
	}
};

class IndexBufferSlot : public IRenderable
{
public:
	IndexBuffer* m_Buffer;

	IndexBufferSlot& operator = (IndexBuffer& buffer)
	{
		m_Buffer = &buffer;
		return *this;
	}

	inline void Render()
	{
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetIndexBuffer( m_Buffer->m_Buffer, DXGI_FORMAT_R16_UINT, offset );
	}
};


template <class T>
class VertexBuffer : public Buffer, public IRenderable
{
public:
	int m_VertexCount;
	std::vector<T> m_Vertices;

	void Reset()
	{
		m_Vertices.clear();
		m_VertexCount = 0;
		m_Buffer->Release();
		m_Buffer = NULL;
	}

	void AddVertex( T& v )
	{
		m_Vertices.push_back( v );
		m_VertexCount = m_Vertices.size();
	}

	void Set()
	{
		// Set vertex buffer
		UINT stride = sizeof(T);
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetVertexBuffers( 0, 1, &m_Buffer, &stride, &offset );

		// Set primitive topology
		Engine.m_Renderer.m_Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	}

	void Update()
	{
		Engine.m_Renderer.m_Context->UpdateSubresource( m_Buffer, 0, NULL, m_Vertices.data(), 0, 0 );
	}

	void Render()
	{
		Set();
		Engine.m_Renderer.m_Context->Draw( m_VertexCount, 0 );
	}

	void Create()
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T)* m_VertexCount;	// from sortOfMesh
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = m_Vertices.data();	// from sortOfMesh	

		HRESULT hr = Engine.m_Renderer.m_Device->CreateBuffer( &bd, &InitData, &m_Buffer );
		if ( FAILED( hr ) )
			throw exception( "Create vertex buffer failed" );
	}

	void Clear()
	{
		m_VertexCount = 0;
		m_Vertices.clear();
	}
};

template <class T, int Slot>
class VertexBufferSlot : public IRenderable
{
public:
	VertexBuffer<T>* m_Buffer;

	VertexBufferSlot<T, Slot>& operator = (VertexBuffer<T>& buffer)
	{
		m_Buffer = &buffer;
		return *this;
	}

	inline void Render()
	{
		UINT stride = sizeof(T);
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetVertexBuffers( Slot, 1, &m_Buffer->m_Buffer, &stride, &offset );
	}
};

#endif // !BUFFER_H
