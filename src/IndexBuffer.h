#pragma once

class IndexBuffer
{
private:
	unsigned int m_Renderer_ID;
	unsigned int m_Count;
public:/*Size-Bytes............count-Element count(for 6 indeces its 6 where size is the size of all the indices which 4x6=24 bytes)*/
	IndexBuffer(const unsigned int* data, unsigned int count);/*Constructor*/
	~IndexBuffer();/*Destructor*/

	/*Methods*/
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};