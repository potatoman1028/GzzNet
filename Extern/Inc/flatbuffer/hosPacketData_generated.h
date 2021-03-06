// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HOSPACKETDATA_HOS_PACKET_H_
#define FLATBUFFERS_GENERATED_HOSPACKETDATA_HOS_PACKET_H_

#include "flatbuffers/flatbuffers.h"

namespace hos {
namespace Packet {

struct Vec3;

enum MOVE_DIR {
  MOVE_DIR_N = 0,
  MOVE_DIR_S = 1,
  MOVE_DIR_W = 2,
  MOVE_DIR_E = 3,
  MOVE_DIR_N_W = 4,
  MOVE_DIR_N_E = 5,
  MOVE_DIR_S_W = 6,
  MOVE_DIR_S_E = 7,
  MOVE_DIR_ROT_L = 8,
  MOVE_DIR_ROT_R = 9,
  MOVE_DIR_STOP = 10,
  MOVE_DIR_MIN = MOVE_DIR_N,
  MOVE_DIR_MAX = MOVE_DIR_STOP
};

inline const MOVE_DIR (&EnumValuesMOVE_DIR())[11] {
  static const MOVE_DIR values[] = {
    MOVE_DIR_N,
    MOVE_DIR_S,
    MOVE_DIR_W,
    MOVE_DIR_E,
    MOVE_DIR_N_W,
    MOVE_DIR_N_E,
    MOVE_DIR_S_W,
    MOVE_DIR_S_E,
    MOVE_DIR_ROT_L,
    MOVE_DIR_ROT_R,
    MOVE_DIR_STOP
  };
  return values;
}

inline const char * const *EnumNamesMOVE_DIR() {
  static const char * const names[12] = {
    "N",
    "S",
    "W",
    "E",
    "N_W",
    "N_E",
    "S_W",
    "S_E",
    "ROT_L",
    "ROT_R",
    "STOP",
    nullptr
  };
  return names;
}

inline const char *EnumNameMOVE_DIR(MOVE_DIR e) {
  if (flatbuffers::IsOutRange(e, MOVE_DIR_N, MOVE_DIR_STOP)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesMOVE_DIR()[index];
}

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(static_cast<void *>(this), 0, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

}  // namespace Packet
}  // namespace hos

#endif  // FLATBUFFERS_GENERATED_HOSPACKETDATA_HOS_PACKET_H_
