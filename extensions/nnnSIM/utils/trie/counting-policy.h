/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *   Original template by University of California written by
 *   Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 *  counting-policy.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  counting-policy.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with counting-policy.h.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COUNTING_POLICY_3N_H_
#define COUNTING_POLICY_3N_H_

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>

namespace ns3
{
  namespace nnn
  {
    namespace nnnSIM
    {
      /**
       * @brief Traits for policy that just keeps track of number of elements
       * It's doing a rather expensive job, but just in case it needs to be extended later
       */
      struct counting_policy_traits
      {
	/// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
	static std::string GetName () { return "Counting"; }

	struct policy_hook_type : public boost::intrusive::list_member_hook<> {};

	template<class Container>
	struct container_hook
	{
	  // could be class/struct implementation
	  typedef boost::intrusive::member_hook< Container,
	      policy_hook_type,
	      &Container::policy_hook_ > type;
	};

	template<class Base,
	class Container,
	class Hook>
	struct policy
	{
	  typedef typename boost::intrusive::list< Container, Hook > policy_container;

	  // could be just typedef
	  class type : public policy_container
	  {
	  public:
	    typedef Container parent_trie;

	    type (Base &base)
	    : base_ (base)
	    {
	    }

	    inline void
	    update (typename parent_trie::iterator item)
	    {
	      // do nothing
	    }

	    inline bool
	    insert (typename parent_trie::iterator item)
	    {
	      policy_container::push_back (*item);
	      return true;
	    }

	    inline void
	    lookup (typename parent_trie::iterator item)
	    {
	      // do nothing
	    }

	    inline void
	    erase (typename parent_trie::iterator item)
	    {
	      policy_container::erase (policy_container::s_iterator_to (*item));
	    }

	    inline void
	    clear ()
	    {
	      policy_container::clear ();
	    }

	  private:
	    type () : base_(*((Base*)0)) { };

	  private:
	    Base &base_;
	  };
	};
      };

    } // nnnSIM
  } // nnn
} // ns3

#endif // COUNTING_POLICY_3N_H_
