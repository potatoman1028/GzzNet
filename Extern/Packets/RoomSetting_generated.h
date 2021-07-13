// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ROOMSETTING_DB_PACKET_H_
#define FLATBUFFERS_GENERATED_ROOMSETTING_DB_PACKET_H_

#include "flatbuffers/flatbuffers.h"

namespace db {
namespace Packet {

struct RoomSetting;
struct RoomSettingBuilder;

struct RoomSetting FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RoomSettingBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SESSION_ID = 4,
    VT_ROOMID = 6,
    VT_USERID = 8,
    VT_ROOMNAME = 10
  };
  int32_t session_id() const {
    return GetField<int32_t>(VT_SESSION_ID, 0);
  }
  int32_t roomid() const {
    return GetField<int32_t>(VT_ROOMID, 0);
  }
  const flatbuffers::String *userid() const {
    return GetPointer<const flatbuffers::String *>(VT_USERID);
  }
  const flatbuffers::String *roomname() const {
    return GetPointer<const flatbuffers::String *>(VT_ROOMNAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_SESSION_ID) &&
           VerifyField<int32_t>(verifier, VT_ROOMID) &&
           VerifyOffset(verifier, VT_USERID) &&
           verifier.VerifyString(userid()) &&
           VerifyOffset(verifier, VT_ROOMNAME) &&
           verifier.VerifyString(roomname()) &&
           verifier.EndTable();
  }
};

struct RoomSettingBuilder {
  typedef RoomSetting Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_session_id(int32_t session_id) {
    fbb_.AddElement<int32_t>(RoomSetting::VT_SESSION_ID, session_id, 0);
  }
  void add_roomid(int32_t roomid) {
    fbb_.AddElement<int32_t>(RoomSetting::VT_ROOMID, roomid, 0);
  }
  void add_userid(flatbuffers::Offset<flatbuffers::String> userid) {
    fbb_.AddOffset(RoomSetting::VT_USERID, userid);
  }
  void add_roomname(flatbuffers::Offset<flatbuffers::String> roomname) {
    fbb_.AddOffset(RoomSetting::VT_ROOMNAME, roomname);
  }
  explicit RoomSettingBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  RoomSettingBuilder &operator=(const RoomSettingBuilder &);
  flatbuffers::Offset<RoomSetting> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RoomSetting>(end);
    return o;
  }
};

inline flatbuffers::Offset<RoomSetting> CreateRoomSetting(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t session_id = 0,
    int32_t roomid = 0,
    flatbuffers::Offset<flatbuffers::String> userid = 0,
    flatbuffers::Offset<flatbuffers::String> roomname = 0) {
  RoomSettingBuilder builder_(_fbb);
  builder_.add_roomname(roomname);
  builder_.add_userid(userid);
  builder_.add_roomid(roomid);
  builder_.add_session_id(session_id);
  return builder_.Finish();
}

inline flatbuffers::Offset<RoomSetting> CreateRoomSettingDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t session_id = 0,
    int32_t roomid = 0,
    const char *userid = nullptr,
    const char *roomname = nullptr) {
  auto userid__ = userid ? _fbb.CreateString(userid) : 0;
  auto roomname__ = roomname ? _fbb.CreateString(roomname) : 0;
  return db::Packet::CreateRoomSetting(
      _fbb,
      session_id,
      roomid,
      userid__,
      roomname__);
}

inline const db::Packet::RoomSetting *GetRoomSetting(const void *buf) {
  return flatbuffers::GetRoot<db::Packet::RoomSetting>(buf);
}

inline const db::Packet::RoomSetting *GetSizePrefixedRoomSetting(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<db::Packet::RoomSetting>(buf);
}

inline bool VerifyRoomSettingBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<db::Packet::RoomSetting>(nullptr);
}

inline bool VerifySizePrefixedRoomSettingBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<db::Packet::RoomSetting>(nullptr);
}

inline void FinishRoomSettingBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<db::Packet::RoomSetting> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedRoomSettingBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<db::Packet::RoomSetting> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Packet
}  // namespace db

#endif  // FLATBUFFERS_GENERATED_ROOMSETTING_DB_PACKET_H_