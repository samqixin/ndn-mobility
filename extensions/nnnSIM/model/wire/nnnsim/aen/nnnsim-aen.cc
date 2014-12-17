/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnnsim-aen.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnnsim-aen.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnnsim-aen.cc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nnnsim-aen.h"

NNN_NAMESPACE_BEGIN

namespace wire {
  namespace nnnSIM {

    NS_OBJECT_ENSURE_REGISTERED (AEN);

    NS_LOG_COMPONENT_DEFINE ("nnn.wire.nnnSIM.AEN");

    AEN::AEN ()
    : CommonHeader<nnn::AEN>()
    {
    }

    AEN::AEN (Ptr<nnn::AEN> aen_p)
    : CommonHeader<nnn::AEN> (aen_p)
    {
    }

    TypeId
    AEN::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::nnn::AEN::nnnSIM")
	  .SetGroupName ("Nnn")
	  .SetParent<Header> ()
	  .AddConstructor<AEN> ()
	  ;
      return tid;
    }

    TypeId
    AEN::GetInstanceTypeId (void) const
    {
      return GetTypeId ();
    }

    Ptr<Packet>
    AEN::ToWire (Ptr<const nnn::AEN> aen_p)
    {
      Ptr<const Packet> p = aen_p->GetWire ();
      if (!p)
	{
	  // Mechanism packets have no payload, make an empty packet
	  Ptr<Packet> packet = Create<Packet> ();
	  AEN wireEncoding (ConstCast<nnn::AEN> (aen_p));
	  packet->AddHeader (wireEncoding);
	  aen_p->SetWire (packet);

	  p = packet;
	}
      return p->Copy ();
    }

    Ptr<nnn::AEN>
    AEN::FromWire (Ptr<Packet> packet)
    {
      Ptr<nnn::AEN> aen_p = Create<nnn::AEN> ();
      Ptr<Packet> wire = packet->Copy ();

      AEN wireEncoding (aen_p);
      packet->RemoveHeader (wireEncoding);

      // Mechanism packets have no payload, make an empty packet
      aen_p->SetWire (wire);

      return aen_p;
    }

    uint32_t
    AEN::GetSerializedSize (void) const
    {
      size_t size =
	  CommonGetSerializedSize () +                    /* Common header */
	  2 +                                             /* Lease time */
	  NnnSim::SerializedSizeName (m_ptr->GetName ()); /* Name size */

      return size;
    }

    void
    AEN::Serialize (Buffer::Iterator start) const
    {
      // Serialize the header
      CommonSerialize(start);

      // Remember that CommonSerialize doesn't write the Packet length
      // Move the iterator forward
      start.Next(CommonGetSerializedSize() -2);

      NS_LOG_INFO ("Serialize -> PktID = " << m_ptr->GetPacketId());
      NS_LOG_INFO ("Serialize -> TTL = " << Seconds(static_cast<uint16_t> (m_ptr->GetLifetime ().ToInteger (Time::S))));
      NS_LOG_INFO ("Serialize -> Version = " << m_ptr->GetVersion ());
      NS_LOG_INFO ("Serialize -> Pkt Len = " << GetSerializedSize());

      // Serialize packet length
      start.WriteU16(GetSerializedSize());

      uint16_t seconds = static_cast<uint16_t> (m_ptr->GetLeasetime ().ToInteger (Time::S));

      NS_ASSERT_MSG (0 <= seconds &&
                      seconds < 65535,
                     "Incorrect Lease time (should not be smaller than 0 and larger than 65535");

      // Round lease time to seconds and serialize
      start.WriteU16 (seconds);
      NS_LOG_INFO ("Serialize -> Lease time = " << seconds);

      // Serialize NNN address
      NnnSim::SerializeName(start, m_ptr->GetName());
      NS_LOG_INFO("Finished serialization");
    }

    uint32_t
    AEN::Deserialize (Buffer::Iterator start)
    {
      Buffer::Iterator i = start;

      // Deserialize the header
      uint32_t skip = CommonDeserialize (i);

      NS_LOG_INFO ("Deserialize -> PktID = " << m_ptr->GetPacketId());
      NS_LOG_INFO ("Deserialize -> TTL = " << Seconds(static_cast<uint16_t> (m_ptr->GetLifetime ().ToInteger (Time::S))));
      NS_LOG_INFO ("Deserialize -> Version = " << m_ptr->GetVersion ());
      NS_LOG_INFO ("Deserialize -> Pkt len = " << m_packet_len);

      // Check packet ID
      if (m_ptr->GetPacketId() != nnn::AEN_NNN)
	throw new AENException ();

      // Move the iterator forward
      i.Next(skip);

      uint16_t seconds = i.ReadU16 ();

      NS_LOG_INFO ("Deserialize ->  = Lease time " << seconds);

      // Deserialize and set the lease time
      m_ptr->SetLeasetime (Seconds (seconds));

      // Deserialize the name
      m_ptr->SetName(NnnSim::DeserializeName(i));

      NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

      return i.GetDistanceFrom (start);
    }
  }
}

NNN_NAMESPACE_END
