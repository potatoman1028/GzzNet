// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HOSBASEPACKET_HOS_PACKET_H_
#define FLATBUFFERS_GENERATED_HOSBASEPACKET_HOS_PACKET_H_

#include "flatbuffers/flatbuffers.h"
#include "hosPacketData_generated.h"

namespace hos {
namespace Packet {

struct BaseProtocol;
struct BaseProtocolBuilder;

struct BaseProtocol FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BaseProtocolBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_EVENT = 6,
    VT_RETURN_ = 8
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  uint32_t event() const {
    return GetField<uint32_t>(VT_EVENT, 0);
  }
  uint32_t return_() const {
    return GetField<uint32_t>(VT_RETURN_, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID) &&
           VerifyField<uint32_t>(verifier, VT_EVENT) &&
           VerifyField<uint32_t>(verifier, VT_RETURN_) &&
           verifier.EndTable();
  }
};

struct BaseProtocolBuilder {
  typedef BaseProtocol Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(BaseProtocol::VT_ID, id, 0);
  }
  void add_event(uint32_t event) {
    fbb_.AddElement<uint32_t>(BaseProtocol::VT_EVENT, event, 0);
  }
  void add_return_(uint32_t return_) {
    fbb_.AddElement<uint32_t>(BaseProtocol::VT_RETURN_, return_, 0);
  }
  explicit BaseProtocolBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  BaseProtocolBuilder &operator=(const BaseProtocolBuilder &);
  flatbuffers::Offset<BaseProtocol> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<BaseProtocol>(end);
    return o;
  }
};

inline flatbuffers::Offset<BaseProtocol> CreateBaseProtocol(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    uint32_t event = 0,
    uint32_t return_ = 0) {
  BaseProtocolBuilder builder_(_fbb);
  builder_.add_return_(return_);
  builder_.add_event(event);
  builder_.add_id(id);
  return builder_.Finish();
}

inline const hos::Packet::BaseProtocol *GetBaseProtocol(const void *buf) {
  return flatbuffers::GetRoot<hos::Packet::BaseProtocol>(buf);
}

inline const hos::Packet::BaseProtocol *GetSizePrefixedBaseProtocol(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<hos::Packet::BaseProtocol>(buf);
}

inline bool VerifyBaseProtocolBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<hos::Packet::BaseProtocol>(nullptr);
}

inline bool VerifySizePrefixedBaseProtocolBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<hos::Packet::BaseProtocol>(nullptr);
}

inline void FinishBaseProtocolBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hos::Packet::BaseProtocol> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedBaseProtocolBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hos::Packet::BaseProtocol> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Packet
}  // namespace hos

#endif  // FLATBUFFERS_GENERATED_HOSBASEPACKET_HOS_PACKET_H_
