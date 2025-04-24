///////////////////////////////////////////////////////////////////////////////
//
//	Class: boost_datetime
//
//	Description: Contains composite functions for boost datetime library
//
//
//	Revision history: 
//	Date	Author	Comment
//	***********************
//	040119	migr	Created
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
#include "objio.h"
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
///////////////////////////////////////////////////////////////////////////////
XTL_COMPOSE_ENUM(boost::date_time::weekdays);
///////////////////////////////////////////////////////////////////////////////
class xtl_boost_ptime : public boost::posix_time::ptime
{
public:
	xtl_boost_ptime() : boost::posix_time::ptime(boost::gregorian::date(boost::date_time::not_a_date_time)) {}
	xtl_boost_ptime(const boost::posix_time::ptime& tm) : boost::posix_time::ptime(tm) {}

	template<class TFormat>
	void composite(obj_output<TFormat>& s)
	{
		s.simple(time_.time_count());
	}
	template<class TFormat>
	void composite(obj_input<TFormat>& s)
	{
		time_rep_type::int_type tm;
		s.simple(tm);
		time_ = time_rep_type(tm);
	}
};
