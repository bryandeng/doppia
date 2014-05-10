// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TestData.proto

#ifndef PROTOBUF_TestData_2eproto__INCLUDED
#define PROTOBUF_TestData_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace doppia_protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_TestData_2eproto();
void protobuf_AssignDesc_TestData_2eproto();
void protobuf_ShutdownFile_TestData_2eproto();

class TestData;

// ===================================================================

class TestData : public ::google::protobuf::Message {
 public:
  TestData();
  virtual ~TestData();
  
  TestData(const TestData& from);
  
  inline TestData& operator=(const TestData& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const TestData& default_instance();
  
  void Swap(TestData* other);
  
  // implements Message ----------------------------------------------
  
  TestData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TestData& from);
  void MergeFrom(const TestData& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 int_value = 1;
  inline bool has_int_value() const;
  inline void clear_int_value();
  static const int kIntValueFieldNumber = 1;
  inline ::google::protobuf::int32 int_value() const;
  inline void set_int_value(::google::protobuf::int32 value);
  
  // optional float float_value = 2;
  inline bool has_float_value() const;
  inline void clear_float_value();
  static const int kFloatValueFieldNumber = 2;
  inline float float_value() const;
  inline void set_float_value(float value);
  
  // optional string string_value = 3;
  inline bool has_string_value() const;
  inline void clear_string_value();
  static const int kStringValueFieldNumber = 3;
  inline const ::std::string& string_value() const;
  inline void set_string_value(const ::std::string& value);
  inline void set_string_value(const char* value);
  inline void set_string_value(const char* value, size_t size);
  inline ::std::string* mutable_string_value();
  
  // @@protoc_insertion_point(class_scope:doppia_protobuf.TestData)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 int_value_;
  float float_value_;
  ::std::string* string_value_;
  static const ::std::string _default_string_value_;
  friend void  protobuf_AddDesc_TestData_2eproto();
  friend void protobuf_AssignDesc_TestData_2eproto();
  friend void protobuf_ShutdownFile_TestData_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static TestData* default_instance_;
};
// ===================================================================


// ===================================================================

// TestData

// optional int32 int_value = 1;
inline bool TestData::has_int_value() const {
  return _has_bit(0);
}
inline void TestData::clear_int_value() {
  int_value_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 TestData::int_value() const {
  return int_value_;
}
inline void TestData::set_int_value(::google::protobuf::int32 value) {
  _set_bit(0);
  int_value_ = value;
}

// optional float float_value = 2;
inline bool TestData::has_float_value() const {
  return _has_bit(1);
}
inline void TestData::clear_float_value() {
  float_value_ = 0;
  _clear_bit(1);
}
inline float TestData::float_value() const {
  return float_value_;
}
inline void TestData::set_float_value(float value) {
  _set_bit(1);
  float_value_ = value;
}

// optional string string_value = 3;
inline bool TestData::has_string_value() const {
  return _has_bit(2);
}
inline void TestData::clear_string_value() {
  if (string_value_ != &_default_string_value_) {
    string_value_->clear();
  }
  _clear_bit(2);
}
inline const ::std::string& TestData::string_value() const {
  return *string_value_;
}
inline void TestData::set_string_value(const ::std::string& value) {
  _set_bit(2);
  if (string_value_ == &_default_string_value_) {
    string_value_ = new ::std::string;
  }
  string_value_->assign(value);
}
inline void TestData::set_string_value(const char* value) {
  _set_bit(2);
  if (string_value_ == &_default_string_value_) {
    string_value_ = new ::std::string;
  }
  string_value_->assign(value);
}
inline void TestData::set_string_value(const char* value, size_t size) {
  _set_bit(2);
  if (string_value_ == &_default_string_value_) {
    string_value_ = new ::std::string;
  }
  string_value_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* TestData::mutable_string_value() {
  _set_bit(2);
  if (string_value_ == &_default_string_value_) {
    string_value_ = new ::std::string;
  }
  return string_value_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace doppia_protobuf

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TestData_2eproto__INCLUDED
