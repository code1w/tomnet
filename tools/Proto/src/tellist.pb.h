// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: tellist.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_tellist_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_tellist_2eproto

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_tellist_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_tellist_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_tellist_2eproto;
namespace Tom {
class personal_info;
class personal_infoDefaultTypeInternal;
extern personal_infoDefaultTypeInternal _personal_info_default_instance_;
class personal_info_list;
class personal_info_listDefaultTypeInternal;
extern personal_info_listDefaultTypeInternal _personal_info_list_default_instance_;
}  // namespace Tom
PROTOBUF_NAMESPACE_OPEN
template<> ::Tom::personal_info* Arena::CreateMaybeMessage<::Tom::personal_info>(Arena*);
template<> ::Tom::personal_info_list* Arena::CreateMaybeMessage<::Tom::personal_info_list>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace Tom {

enum gender_type : int {
  MALE = 0,
  FEMALE = 1,
  OTHER = 2,
  gender_type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  gender_type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool gender_type_IsValid(int value);
constexpr gender_type gender_type_MIN = MALE;
constexpr gender_type gender_type_MAX = OTHER;
constexpr int gender_type_ARRAYSIZE = gender_type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* gender_type_descriptor();
template<typename T>
inline const std::string& gender_type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, gender_type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function gender_type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    gender_type_descriptor(), enum_t_value);
}
inline bool gender_type_Parse(
    const std::string& name, gender_type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<gender_type>(
    gender_type_descriptor(), name, value);
}
// ===================================================================

class personal_info PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Tom.personal_info) */ {
 public:
  inline personal_info() : personal_info(nullptr) {};
  virtual ~personal_info();

  personal_info(const personal_info& from);
  personal_info(personal_info&& from) noexcept
    : personal_info() {
    *this = ::std::move(from);
  }

  inline personal_info& operator=(const personal_info& from) {
    CopyFrom(from);
    return *this;
  }
  inline personal_info& operator=(personal_info&& from) noexcept {
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
  static const personal_info& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const personal_info* internal_default_instance() {
    return reinterpret_cast<const personal_info*>(
               &_personal_info_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(personal_info& a, personal_info& b) {
    a.Swap(&b);
  }
  inline void Swap(personal_info* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(personal_info* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline personal_info* New() const final {
    return CreateMaybeMessage<personal_info>(nullptr);
  }

  personal_info* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<personal_info>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const personal_info& from);
  void MergeFrom(const personal_info& from);
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
  void InternalSwap(personal_info* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Tom.personal_info";
  }
  protected:
  explicit personal_info(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_tellist_2eproto);
    return ::descriptor_table_tellist_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 2,
    kIdFieldNumber = 1,
    kAgeFieldNumber = 3,
    kPhoneNumFieldNumber = 5,
    kGenderFieldNumber = 4,
  };
  // string name = 2;
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

  // uint32 id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 age = 3;
  void clear_age();
  ::PROTOBUF_NAMESPACE_ID::uint32 age() const;
  void set_age(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_age() const;
  void _internal_set_age(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint64 phone_num = 5;
  void clear_phone_num();
  ::PROTOBUF_NAMESPACE_ID::uint64 phone_num() const;
  void set_phone_num(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_phone_num() const;
  void _internal_set_phone_num(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // .Tom.gender_type gender = 4;
  void clear_gender();
  ::Tom::gender_type gender() const;
  void set_gender(::Tom::gender_type value);
  private:
  ::Tom::gender_type _internal_gender() const;
  void _internal_set_gender(::Tom::gender_type value);
  public:

  // @@protoc_insertion_point(class_scope:Tom.personal_info)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::uint32 id_;
  ::PROTOBUF_NAMESPACE_ID::uint32 age_;
  ::PROTOBUF_NAMESPACE_ID::uint64 phone_num_;
  int gender_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_tellist_2eproto;
};
// -------------------------------------------------------------------

class personal_info_list PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Tom.personal_info_list) */ {
 public:
  inline personal_info_list() : personal_info_list(nullptr) {};
  virtual ~personal_info_list();

  personal_info_list(const personal_info_list& from);
  personal_info_list(personal_info_list&& from) noexcept
    : personal_info_list() {
    *this = ::std::move(from);
  }

  inline personal_info_list& operator=(const personal_info_list& from) {
    CopyFrom(from);
    return *this;
  }
  inline personal_info_list& operator=(personal_info_list&& from) noexcept {
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
  static const personal_info_list& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const personal_info_list* internal_default_instance() {
    return reinterpret_cast<const personal_info_list*>(
               &_personal_info_list_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(personal_info_list& a, personal_info_list& b) {
    a.Swap(&b);
  }
  inline void Swap(personal_info_list* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(personal_info_list* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline personal_info_list* New() const final {
    return CreateMaybeMessage<personal_info_list>(nullptr);
  }

  personal_info_list* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<personal_info_list>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const personal_info_list& from);
  void MergeFrom(const personal_info_list& from);
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
  void InternalSwap(personal_info_list* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Tom.personal_info_list";
  }
  protected:
  explicit personal_info_list(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_tellist_2eproto);
    return ::descriptor_table_tellist_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kInfoFieldNumber = 1,
  };
  // repeated .Tom.personal_info info = 1;
  int info_size() const;
  private:
  int _internal_info_size() const;
  public:
  void clear_info();
  ::Tom::personal_info* mutable_info(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Tom::personal_info >*
      mutable_info();
  private:
  const ::Tom::personal_info& _internal_info(int index) const;
  ::Tom::personal_info* _internal_add_info();
  public:
  const ::Tom::personal_info& info(int index) const;
  ::Tom::personal_info* add_info();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Tom::personal_info >&
      info() const;

  // @@protoc_insertion_point(class_scope:Tom.personal_info_list)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Tom::personal_info > info_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_tellist_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// personal_info

// uint32 id = 1;
inline void personal_info::clear_id() {
  id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 personal_info::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 personal_info::id() const {
  // @@protoc_insertion_point(field_get:Tom.personal_info.id)
  return _internal_id();
}
inline void personal_info::_internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  id_ = value;
}
inline void personal_info::set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:Tom.personal_info.id)
}

// string name = 2;
inline void personal_info::clear_name() {
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& personal_info::name() const {
  // @@protoc_insertion_point(field_get:Tom.personal_info.name)
  return _internal_name();
}
inline void personal_info::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:Tom.personal_info.name)
}
inline std::string* personal_info::mutable_name() {
  // @@protoc_insertion_point(field_mutable:Tom.personal_info.name)
  return _internal_mutable_name();
}
inline const std::string& personal_info::_internal_name() const {
  return name_.Get();
}
inline void personal_info::_internal_set_name(const std::string& value) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void personal_info::set_name(std::string&& value) {
  
  name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:Tom.personal_info.name)
}
inline void personal_info::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:Tom.personal_info.name)
}
inline void personal_info::set_name(const char* value,
    size_t size) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:Tom.personal_info.name)
}
inline std::string* personal_info::_internal_mutable_name() {
  
  return name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* personal_info::release_name() {
  // @@protoc_insertion_point(field_release:Tom.personal_info.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void personal_info::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:Tom.personal_info.name)
}
inline std::string* personal_info::unsafe_arena_release_name() {
  // @@protoc_insertion_point(field_unsafe_arena_release:Tom.personal_info.name)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return name_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void personal_info::unsafe_arena_set_allocated_name(
    std::string* name) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (name != nullptr) {
    
  } else {
    
  }
  name_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      name, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:Tom.personal_info.name)
}

// uint32 age = 3;
inline void personal_info::clear_age() {
  age_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 personal_info::_internal_age() const {
  return age_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 personal_info::age() const {
  // @@protoc_insertion_point(field_get:Tom.personal_info.age)
  return _internal_age();
}
inline void personal_info::_internal_set_age(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  age_ = value;
}
inline void personal_info::set_age(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_age(value);
  // @@protoc_insertion_point(field_set:Tom.personal_info.age)
}

// .Tom.gender_type gender = 4;
inline void personal_info::clear_gender() {
  gender_ = 0;
}
inline ::Tom::gender_type personal_info::_internal_gender() const {
  return static_cast< ::Tom::gender_type >(gender_);
}
inline ::Tom::gender_type personal_info::gender() const {
  // @@protoc_insertion_point(field_get:Tom.personal_info.gender)
  return _internal_gender();
}
inline void personal_info::_internal_set_gender(::Tom::gender_type value) {
  
  gender_ = value;
}
inline void personal_info::set_gender(::Tom::gender_type value) {
  _internal_set_gender(value);
  // @@protoc_insertion_point(field_set:Tom.personal_info.gender)
}

// uint64 phone_num = 5;
inline void personal_info::clear_phone_num() {
  phone_num_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 personal_info::_internal_phone_num() const {
  return phone_num_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 personal_info::phone_num() const {
  // @@protoc_insertion_point(field_get:Tom.personal_info.phone_num)
  return _internal_phone_num();
}
inline void personal_info::_internal_set_phone_num(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  phone_num_ = value;
}
inline void personal_info::set_phone_num(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_phone_num(value);
  // @@protoc_insertion_point(field_set:Tom.personal_info.phone_num)
}

// -------------------------------------------------------------------

// personal_info_list

// repeated .Tom.personal_info info = 1;
inline int personal_info_list::_internal_info_size() const {
  return info_.size();
}
inline int personal_info_list::info_size() const {
  return _internal_info_size();
}
inline void personal_info_list::clear_info() {
  info_.Clear();
}
inline ::Tom::personal_info* personal_info_list::mutable_info(int index) {
  // @@protoc_insertion_point(field_mutable:Tom.personal_info_list.info)
  return info_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Tom::personal_info >*
personal_info_list::mutable_info() {
  // @@protoc_insertion_point(field_mutable_list:Tom.personal_info_list.info)
  return &info_;
}
inline const ::Tom::personal_info& personal_info_list::_internal_info(int index) const {
  return info_.Get(index);
}
inline const ::Tom::personal_info& personal_info_list::info(int index) const {
  // @@protoc_insertion_point(field_get:Tom.personal_info_list.info)
  return _internal_info(index);
}
inline ::Tom::personal_info* personal_info_list::_internal_add_info() {
  return info_.Add();
}
inline ::Tom::personal_info* personal_info_list::add_info() {
  // @@protoc_insertion_point(field_add:Tom.personal_info_list.info)
  return _internal_add_info();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::Tom::personal_info >&
personal_info_list::info() const {
  // @@protoc_insertion_point(field_list:Tom.personal_info_list.info)
  return info_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Tom

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Tom::gender_type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Tom::gender_type>() {
  return ::Tom::gender_type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_tellist_2eproto
