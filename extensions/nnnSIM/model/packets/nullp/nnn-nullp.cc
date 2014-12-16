/* -*- Mode: C++; c-file-style: "gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nullp.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nullp.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nullp.cc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/unused.h>

#include "nnn-nullp.h"

NS_LOG_COMPONENT_DEFINE ("nnn.NULLp");

namespace ns3 {
  namespace nnn {

    NULLp::NULLp ()
    : NNNPacket (NULL_NNN, Seconds (0))
    , m_payload  (Create<Packet> ())
    , m_PDUdatatype (NDN_NNN)
    {
    }

    NULLp::NULLp (Ptr<Packet> payload)
    : NNNPacket (NULL_NNN, Seconds (0))
    , m_PDUdatatype (NDN_NNN)
    {
      if (m_payload == 0)
	{
	  m_payload = Create<Packet> ();
	} else
	  {
	    m_payload = payload;
	  }
    }

    NULLp::NULLp (const NULLp &nullp)
    : NNNPacket (NULL_NNN, nullp.GetLifetime ())
    , m_payload  (nullp.GetPayload ()->Copy ())
    , m_PDUdatatype (nullp.GetPDUPayloadType ())
    {
      NS_LOG_FUNCTION("NULLp correct copy constructor");

      SetWire (nullp.GetWire ());
    }

    uint16_t
    NULLp::GetPDUPayloadType() const
    {
      return m_PDUdatatype;
    }

    void
    NULLp::SetPDUPayloadType (uint16_t pdu_type)
    {
      m_PDUdatatype = pdu_type;
    }

    void
    NULLp::SetPayload (Ptr<Packet> payload)
    {
      m_payload = payload;
      m_wire = 0;
    }

    Ptr<const Packet>
    NULLp::GetPayload () const
    {
      return m_payload;
    }

    void
    NULLp::Print (std::ostream &os) const
    {
      os << "<NULLp>\n";
      os << "  <TTL>" << GetLifetime () << "</TTL>\n";
      if (m_payload != 0)
	{
	  os << "  <Payload>Yes</Payload>\n";
	} else
	  {
	    os << "  <Payload>No</Payload>\n";
	  }
      os << "</NULLp>";
    }
  } // namespace nnn
} // namespace ns3
