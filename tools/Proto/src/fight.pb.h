// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: fight.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_fight_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_fight_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_fight_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_fight_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_fight_2eproto;
namespace msg {
class EnterFight;
class EnterFightDefaultTypeInternal;
extern EnterFightDefaultTypeInternal _EnterFight_default_instance_;
class FightResult;
class FightResultDefaultTypeInternal;
extern FightResultDefaultTypeInternal _FightResult_default_instance_;
class StartFight;
class StartFightDefaultTypeInternal;
extern StartFightDefaultTypeInternal _StartFight_default_instance_;
}  // namespace msg
PROTOBUF_NAMESPACE_OPEN
template<> ::msg::EnterFight* Arena::CreateMaybeMessage<::msg::EnterFight>(Arena*);
template<> ::msg::FightResult* Arena::CreateMaybeMessage<::msg::FightResult>(Arena*);
template<> ::msg::StartFight* Arena::CreateMaybeMessage<::msg::StartFight>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace msg {

// ===================================================================

class StartFight PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.StartFight) */ {
 public:
  inline StartFight() : StartFight(nullptr) {};
  virtual ~StartFight();

  StartFight(const StartFight& from);
  StartFight(StartFight&& from) noexcept
    : StartFight() {
    *this = ::std::move(from);
  }

  inline StartFight& operator=(const StartFight& from) {
    CopyFrom(from);
    return *this;
  }
  inline StartFight& operator=(StartFight&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const StartFight& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const StartFight* internal_default_instance() {
    return reinterpret_cast<const StartFight*>(
               &_StartFight_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(StartFight& a, StartFight& b) {
    a.Swap(&b);
  }
  inline void Swap(StartFight* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(StartFight* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline StartFight* New() const final {
    return CreateMaybeMessage<StartFight>(nullptr);
  }

  StartFight* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<StartFight>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const StartFight& from);
  void MergeFrom(const StartFight& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(StartFight* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.StartFight";
  }
  protected:
  explicit StartFight(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_fight_2eproto);
    return ::descriptor_table_fight_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kErrorCodeFieldNumber = 1,
    kVersionFieldNumber = 2,
  };
  // int32 errorCode = 1;
  void clear_errorcode();
  ::PROTOBUF_NAMESPACE_ID::int32 errorcode() const;
  void set_errorcode(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_errorcode() const;
  void _internal_set_errorcode(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // float version = 2;
  void clear_version();
  float version() const;
  void set_version(float value);
  private:
  float _internal_version() const;
  void _internal_set_version(float value);
  public:

  // @@protoc_insertion_point(class_scope:msg.StartFight)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::int32 errorcode_;
  float version_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_fight_2eproto;
};
// -------------------------------------------------------------------

class FightResult PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.FightResult) */ {
 public:
  inline FightResult() : FightResult(nullptr) {};
  virtual ~FightResult();

  FightResult(const FightResult& from);
  FightResult(FightResult&& from) noexcept
    : FightResult() {
    *this = ::std::move(from);
  }

  inline FightResult& operator=(const FightResult& from) {
    CopyFrom(from);
    return *this;
  }
  inline FightResult& operator=(FightResult&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const FightResult& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const FightResult* internal_default_instance() {
    return reinterpret_cast<const FightResult*>(
               &_FightResult_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(FightResult& a, FightResult& b) {
    a.Swap(&b);
  }
  inline void Swap(FightResult* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(FightResult* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline FightResult* New() const final {
    return CreateMaybeMessage<FightResult>(nullptr);
  }

  FightResult* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<FightResult>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const FightResult& from);
  void MergeFrom(const FightResult& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(FightResult* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.FightResult";
  }
  protected:
  explicit FightResult(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_fight_2eproto);
    return ::descriptor_table_fight_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kContentFieldNumber = 2,
  };
  // string name = 1;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_name();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_name(
      std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // string content = 2;
  void clear_content();
  const std::string& content() const;
  void set_content(const std::string& value);
  void set_content(std::string&& value);
  void set_content(const char* value);
  void set_content(const char* value, size_t size);
  std::string* mutable_content();
  std::string* release_content();
  void set_allocated_content(std::string* content);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_content();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_content(
      std::string* content);
  private:
  const std::string& _internal_content() const;
  void _internal_set_content(const std::string& value);
  std::string* _internal_mutable_content();
  public:

  // @@protoc_insertion_point(class_scope:msg.FightResult)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr content_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_fight_2eproto;
};
// -------------------------------------------------------------------

class EnterFight PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.EnterFight) */ {
 public:
  inline EnterFight() : EnterFight(nullptr) {};
  virtual ~EnterFight();

  EnterFight(const EnterFight& from);
  EnterFight(EnterFight&& from) noexcept
    : EnterFight() {
    *this = ::std::move(from);
  }

  inline EnterFight& operator=(const EnterFight& from) {
    CopyFrom(from);
    return *this;
  }
  inline EnterFight& operator=(EnterFight&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const EnterFight& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const EnterFight* internal_default_instance() {
    return reinterpret_cast<const EnterFight*>(
               &_EnterFight_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(EnterFight& a, EnterFight& b) {
    a.Swap(&b);
  }
  inline void Swap(EnterFight* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(EnterFight* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline EnterFight* New() const final {
    return CreateMaybeMessage<EnterFight>(nullptr);
  }

  EnterFight* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<EnterFight>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const EnterFight& from);
  void MergeFrom(const EnterFight& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(EnterFight* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.EnterFight";
  }
  protected:
  explicit EnterFight(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_fight_2eproto);
    return ::descriptor_table_fight_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kContentFieldNumber = 2,
  };
  // string name = 1;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_name();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_name(
      std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // string content = 2;
  void clear_content();
  const std::string& content() const;
  void set_content(const std::string& value);
  void set_content(std::string&& value);
  void set_content(const char* value);
  void set_content(const char* value, size_t size);
  std::string* mutable_content();
  std::string* release_content();
  void set_allocated_content(std::string* content);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_content();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_content(
      std::string* content);
  private:
  const std::string& _internal_content() const;
  void _internal_set_content(const std::string& value);
  std::string* _internal_mutable_content();
  public:

  // @@protoc_insertion_point(class_scope:msg.EnterFight)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr content_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_fight_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// StartFight

// int32 errorCode = 1;
inline void StartFight::clear_errorcode() {
  errorcode_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 StartFight::_internal_errorcode() const {
  return errorcode_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 StartFight::errorcode() const {
  // @@protoc_insertion_point(field_get:msg.StartFight.errorCode)
  return _internal_errorcode();
}
inline void StartFight::_internal_set_errorcode(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  errorcode_ = value;
}
inline void StartFight::set_errorcode(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_errorcode(value);
  // @@protoc_insertion_point(field_set:msg.StartFight.errorCode)
}

// float version = 2;
inline void StartFight::clear_version() {
  version_ = 0;
}
inline float StartFight::_internal_version() const {
  return version_;
}
inline float StartFight::version() const {
  // @@protoc_insertion_point(field_get:msg.StartFight.version)
  return _internal_version();
}
inline void StartFight::_internal_set_version(float value) {
  
  version_ = value;
}
inline void StartFight::set_version(float value) {
  _internal_set_version(value);
  // @@protoc_insertion_point(field_set:msg.StartFight.version)
}

// -------------------------------------------------------------------

// FightResult

// string name = 1;
inline void FightResult::clear_name() {
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& FightResult::name() const {
  // @@protoc_insertion_point(field_get:msg.FightResult.name)
  return _internal_name();
}
inline void FightResult::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:msg.FightResult.name)
}
inline std::string* FightResult::mutable_name() {
  // @@protoc_insertion_point(field_mutable:msg.FightResult.name)
  return _internal_mutable_name();
}
inline const std::string& FightResult::_internal_name() const {
  return name_.Get();
}
inline void FightResult::_internal_set_name(const std::string& value) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void FightResult::set_name(std::string&& value) {
  
  name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.FightResult.name)
}
inline void FightResult::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.FightResult.name)
}
inline void FightResult::set_name(const char* value,
    size_t size) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.FightResult.name)
}
inline std::string* FightResult::_internal_mutable_name() {
  
  return name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* FightResult::release_name() {
  // @@protoc_insertion_point(field_release:msg.FightResult.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void FightResult::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.FightResult.name)
}
inline std::string* FightResult::unsafe_arena_release_name() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.FightResult.name)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return name_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void FightResult::unsafe_arena_set_allocated_name(
    std::string* name) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (name != nullptr) {
    
  } else {
    
  }
  name_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      name, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.FightResult.name)
}

// string content = 2;
inline void FightResult::clear_content() {
  content_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& FightResult::content() const {
  // @@protoc_insertion_point(field_get:msg.FightResult.content)
  return _internal_content();
}
inline void FightResult::set_content(const std::string& value) {
  _internal_set_content(value);
  // @@protoc_insertion_point(field_set:msg.FightResult.content)
}
inline std::string* FightResult::mutable_content() {
  // @@protoc_insertion_point(field_mutable:msg.FightResult.content)
  return _internal_mutable_content();
}
inline const std::string& FightResult::_internal_content() const {
  return content_.Get();
}
inline void FightResult::_internal_set_content(const std::string& value) {
  
  content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void FightResult::set_content(std::string&& value) {
  
  content_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.FightResult.content)
}
inline void FightResult::set_content(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.FightResult.content)
}
inline void FightResult::set_content(const char* value,
    size_t size) {
  
  content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.FightResult.content)
}
inline std::string* FightResult::_internal_mutable_content() {
  
  return content_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* FightResult::release_content() {
  // @@protoc_insertion_point(field_release:msg.FightResult.content)
  return content_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void FightResult::set_allocated_content(std::string* content) {
  if (content != nullptr) {
    
  } else {
    
  }
  content_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), content,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.FightResult.content)
}
inline std::string* FightResult::unsafe_arena_release_content() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.FightResult.content)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return content_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void FightResult::unsafe_arena_set_allocated_content(
    std::string* content) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (content != nullptr) {
    
  } else {
    
  }
  content_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      content, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.FightResult.content)
}

// -------------------------------------------------------------------

// EnterFight

// string name = 1;
inline void EnterFight::clear_name() {
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& EnterFight::name() const {
  // @@protoc_insertion_point(field_get:msg.EnterFight.name)
  return _internal_name();
}
inline void EnterFight::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:msg.EnterFight.name)
}
inline std::string* EnterFight::mutable_name() {
  // @@protoc_insertion_point(field_mutable:msg.EnterFight.name)
  return _internal_mutable_name();
}
inline const std::string& EnterFight::_internal_name() const {
  return name_.Get();
}
inline void EnterFight::_internal_set_name(const std::string& value) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void EnterFight::set_name(std::string&& value) {
  
  name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.EnterFight.name)
}
inline void EnterFight::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.EnterFight.name)
}
inline void EnterFight::set_name(const char* value,
    size_t size) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.EnterFight.name)
}
inline std::string* EnterFight::_internal_mutable_name() {
  
  return name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* EnterFight::release_name() {
  // @@protoc_insertion_point(field_release:msg.EnterFight.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void EnterFight::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.EnterFight.name)
}
inline std::string* EnterFight::unsafe_arena_release_name() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.EnterFight.name)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return name_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void EnterFight::unsafe_arena_set_allocated_name(
    std::string* name) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (name != nullptr) {
    
  } else {
    
  }
  name_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      name, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.EnterFight.name)
}

// string content = 2;
inline void EnterFight::clear_content() {
  content_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& EnterFight::content() const {
  // @@protoc_insertion_point(field_get:msg.EnterFight.content)
  return _internal_content();
}
inline void EnterFight::set_content(const std::string& value) {
  _internal_set_content(value);
  // @@protoc_insertion_point(field_set:msg.EnterFight.content)
}
inline std::string* EnterFight::mutable_content() {
  // @@protoc_insertion_point(field_mutable:msg.EnterFight.content)
  return _internal_mutable_content();
}
inline const std::string& EnterFight::_internal_content() const {
  return content_.Get();
}
inline void EnterFight::_internal_set_content(const std::string& value) {
  
  content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void EnterFight::set_content(std::string&& value) {
  
  content_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.EnterFight.content)
}
inline void EnterFight::set_content(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.EnterFight.content)
}
inline void EnterFight::set_content(const char* value,
    size_t size) {
  
  content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.EnterFight.content)
}
inline std::string* EnterFight::_internal_mutable_content() {
  
  return content_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* EnterFight::release_content() {
  // @@protoc_insertion_point(field_release:msg.EnterFight.content)
  return content_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void EnterFight::set_allocated_content(std::string* content) {
  if (content != nullptr) {
    
  } else {
    
  }
  content_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), content,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.EnterFight.content)
}
inline std::string* EnterFight::unsafe_arena_release_content() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.EnterFight.content)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return content_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void EnterFight::unsafe_arena_set_allocated_content(
    std::string* content) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (content != nullptr) {
    
  } else {
    
  }
  content_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      content, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.EnterFight.content)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_fight_2eproto
