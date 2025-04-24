///////////////////////////////////////////////////////////////////////////////
//
//	Class: var_buffer
//
//	Description: Buffer of variable size to be used with XTL. The buffer is linkable; i.e. 
//				 you can attach a buffer to another buffer, postponing the copying until it is
//				 absolutely necessary. This makes it possible to prepend a buffer with data without
//				 incurring an expensive copy.
//
//	Revision history: 
//	Date	Author	Comment
//	***********************
//	021003	Migr1	Created
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
#include <boost/shared_array.hpp>
#include <utility>
#include <list>
#include "objio.h"
///////////////////////////////////////////////////////////////////////////////
#define XTL_VAR_BUFFER_MAX_BLOCK_SIZE		512
///////////////////////////////////////////////////////////////////////////////
class var_buffer
{
public:
	typedef boost::shared_array<char>			TBlockData;
	typedef std::pair<TBlockData, std::size_t>	TBlock;
	typedef std::list<TBlock>					TBlockList;

	class buffer_overflow_error : public std::overflow_error 
	{
	public:
		buffer_overflow_error(std::size_t left, std::size_t needed):
			std::overflow_error("XTL var_buffer overflow"),
			     m_nBytesLeft(left), m_nBytesNeeded(needed) {}
	 	std::size_t m_nBytesLeft;
		std::size_t m_nBytesNeeded;
	};

	class out_of_sync_error : public std::runtime_error 
	{
	public:
		out_of_sync_error() : std::runtime_error("var_buffer::composite(): Out of sync") {}
	};

	var_buffer();
	var_buffer(const var_buffer& buf);

	void*				require(std::size_t size);
	void*				desire(std::size_t size);
	
	void				rewind();

	void				copy_from(const var_buffer& buf);

	void				clear();

	std::size_t			size() const;

	TBlockList&			blocks();
	const TBlockList&	blocks() const;

	void composite(obj_output<raw_format<var_buffer> >& output);
	void composite(obj_input<raw_format<var_buffer> >& input);

protected:
	TBlockList				m_listBlocks;
	TBlockList::iterator	m_itInsertBefore;
	TBlockList::iterator	m_itCurrent;
	char*					m_pCurrentPos;
};
///////////////////////////////////////////////////////////////////////////////
// Function implementations
///////////////////////////////////////////////////////////////////////////////
inline var_buffer::var_buffer()
:	m_listBlocks(),
	m_itInsertBefore(m_listBlocks.end()),
	m_itCurrent(m_listBlocks.end()),
	m_pCurrentPos(0)
{
}

inline var_buffer::var_buffer(const var_buffer& buf)
:	m_listBlocks(buf.m_listBlocks),
	m_itInsertBefore(m_listBlocks.begin()),
	m_itCurrent(m_listBlocks.begin()),
	m_pCurrentPos(m_listBlocks.empty() ? 0 : m_listBlocks.front().first.get())
{
}

inline void* var_buffer::require(std::size_t size)
{
	if ( m_itCurrent == m_listBlocks.end() )
		throw buffer_overflow_error(0, size);
	if ( static_cast<std::size_t>(m_pCurrentPos + size - m_itCurrent->first.get()) > m_itCurrent->second )
	{
		if ( static_cast<std::size_t>(m_pCurrentPos - m_itCurrent->first.get()) != m_itCurrent->second )
			throw buffer_overflow_error(m_itCurrent->second - (m_pCurrentPos - m_itCurrent->first.get()), size);
		++m_itCurrent;
		if ( m_itCurrent == m_listBlocks.end() )
			throw buffer_overflow_error(0, size);
		if ( m_itCurrent->second < size )
			throw buffer_overflow_error(m_itCurrent->second, size);
		m_pCurrentPos = m_itCurrent->first.get() + size;
		return m_itCurrent->first.get();
	}
	else
	{
		char* pPos = m_pCurrentPos;
		m_pCurrentPos += size;
		return pPos;
	}
}

inline void* var_buffer::desire(std::size_t size)
{
	if ( m_itCurrent == m_listBlocks.end() || m_itCurrent->second + size > XTL_VAR_BUFFER_MAX_BLOCK_SIZE || m_pCurrentPos == m_listBlocks.front().first.get() )
	{
		m_itCurrent = m_listBlocks.insert(m_itInsertBefore, std::make_pair(TBlockData(new char[XTL_VAR_BUFFER_MAX_BLOCK_SIZE]), size));
		m_pCurrentPos = m_itCurrent->first.get() + size;
		return m_itCurrent->first.get();
	}
	else
	{
		char* pPos = m_pCurrentPos;
		m_itCurrent->second += size;
		m_pCurrentPos += size;
		return pPos;
	}
}

inline var_buffer::TBlockList& var_buffer::blocks()
{
	return m_listBlocks;
}

inline const var_buffer::TBlockList& var_buffer::blocks() const
{
	return m_listBlocks;
}

inline void var_buffer::rewind()
{
	m_itInsertBefore = m_listBlocks.begin();
	m_itCurrent = m_listBlocks.begin();
	if ( m_itCurrent != m_listBlocks.end() )
		m_pCurrentPos = m_itCurrent->first.get();
}

inline void var_buffer::copy_from(const var_buffer& buf)
{
	m_listBlocks.clear();
	m_listBlocks.insert(m_listBlocks.begin(), TBlockList::const_iterator(buf.m_itCurrent), buf.m_listBlocks.end());
	m_itCurrent = m_listBlocks.begin();
	if ( m_itCurrent != m_listBlocks.end() && static_cast<std::size_t>(buf.m_pCurrentPos - m_itCurrent->first.get()) == m_itCurrent->second )
	{
		m_listBlocks.pop_front();
		m_itCurrent = m_listBlocks.begin();
		m_itInsertBefore = m_listBlocks.begin();
		if ( m_itCurrent != m_listBlocks.end() )
			m_pCurrentPos = m_itCurrent->first.get();
	}
	else
	{
		m_itInsertBefore = m_listBlocks.begin();
		m_pCurrentPos = buf.m_pCurrentPos;
	}
}	

inline void var_buffer::clear()
{
	m_listBlocks.clear();
	rewind();
}

inline std::size_t var_buffer::size() const
{
	if ( m_itCurrent == m_listBlocks.end() )
		return 0;
	else
	{
		std::size_t nSize = (m_itCurrent->first.get() + m_itCurrent->second) - m_pCurrentPos;
		TBlockList::iterator it = m_itCurrent;
		for ( ++it ; it != m_listBlocks.end() ; ++it )
			nSize += it->second;
		return nSize;
	}
}

inline void var_buffer::composite(obj_output<raw_format<var_buffer> >& output)
{
	output.simple(m_listBlocks.size());
	if ( !m_listBlocks.empty() )
	{
		var_buffer& bufOutput = output.GetFormat().GetBuffer();
		bufOutput.m_listBlocks.insert(bufOutput.m_itInsertBefore, m_listBlocks.begin(), m_listBlocks.end());
		bufOutput.m_itCurrent = bufOutput.m_listBlocks.end();
		bufOutput.m_pCurrentPos = 0;
	}
}

inline void var_buffer::composite(obj_input<raw_format<var_buffer> >& input)
{
	std::size_t n;
	input.simple(n);
	if ( n > 0 )
	{
		var_buffer& bufInput = input.GetFormat().GetBuffer();

		if ( bufInput.m_itCurrent == bufInput.m_listBlocks.end() 
			|| (std::size_t)(bufInput.m_pCurrentPos - bufInput.m_itCurrent->first.get()) != bufInput.m_itCurrent->second )
			throw out_of_sync_error();

		++bufInput.m_itCurrent;
		do
		{
			if ( bufInput.m_itCurrent == bufInput.m_listBlocks.end() )
				throw out_of_sync_error();

			m_listBlocks.push_back(*bufInput.m_itCurrent);
			++bufInput.m_itCurrent;
			--n;
		} while ( n > 0 );

		bufInput.m_pCurrentPos = bufInput.m_itCurrent == bufInput.m_listBlocks.end() ? 0 : bufInput.m_itCurrent->first.get();
	}

	m_itCurrent = m_itInsertBefore = m_listBlocks.begin();
	m_pCurrentPos = m_itCurrent == m_listBlocks.end() ? 0 : m_itCurrent->first.get();
}
