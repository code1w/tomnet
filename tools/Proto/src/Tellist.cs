// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: tellist.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Tom {

  /// <summary>Holder for reflection information generated from tellist.proto</summary>
  public static partial class TellistReflection {

    #region Descriptor
    /// <summary>File descriptor for tellist.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static TellistReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Cg10ZWxsaXN0LnByb3RvEgNUb20iawoNcGVyc29uYWxfaW5mbxIKCgJpZBgB",
            "IAEoDRIMCgRuYW1lGAIgASgJEgsKA2FnZRgDIAEoDRIgCgZnZW5kZXIYBCAB",
            "KA4yEC5Ub20uZ2VuZGVyX3R5cGUSEQoJcGhvbmVfbnVtGAUgASgEIjYKEnBl",
            "cnNvbmFsX2luZm9fbGlzdBIgCgRpbmZvGAEgAygLMhIuVG9tLnBlcnNvbmFs",
            "X2luZm8qLgoLZ2VuZGVyX3R5cGUSCAoETUFMRRAAEgoKBkZFTUFMRRABEgkK",
            "BU9USEVSEAJiBnByb3RvMw=="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(new[] {typeof(global::Tom.gender_type), }, null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Tom.personal_info), global::Tom.personal_info.Parser, new[]{ "Id", "Name", "Age", "Gender", "PhoneNum" }, null, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Tom.personal_info_list), global::Tom.personal_info_list.Parser, new[]{ "Info" }, null, null, null, null)
          }));
    }
    #endregion

  }
  #region Enums
  public enum gender_type {
    [pbr::OriginalName("MALE")] Male = 0,
    [pbr::OriginalName("FEMALE")] Female = 1,
    [pbr::OriginalName("OTHER")] Other = 2,
  }

  #endregion

  #region Messages
  public sealed partial class personal_info : pb::IMessage<personal_info> {
    private static readonly pb::MessageParser<personal_info> _parser = new pb::MessageParser<personal_info>(() => new personal_info());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<personal_info> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Tom.TellistReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public personal_info() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public personal_info(personal_info other) : this() {
      id_ = other.id_;
      name_ = other.name_;
      age_ = other.age_;
      gender_ = other.gender_;
      phoneNum_ = other.phoneNum_;
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public personal_info Clone() {
      return new personal_info(this);
    }

    /// <summary>Field number for the "id" field.</summary>
    public const int IdFieldNumber = 1;
    private uint id_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Id {
      get { return id_; }
      set {
        id_ = value;
      }
    }

    /// <summary>Field number for the "name" field.</summary>
    public const int NameFieldNumber = 2;
    private string name_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Name {
      get { return name_; }
      set {
        name_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    /// <summary>Field number for the "age" field.</summary>
    public const int AgeFieldNumber = 3;
    private uint age_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Age {
      get { return age_; }
      set {
        age_ = value;
      }
    }

    /// <summary>Field number for the "gender" field.</summary>
    public const int GenderFieldNumber = 4;
    private global::Tom.gender_type gender_ = global::Tom.gender_type.Male;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Tom.gender_type Gender {
      get { return gender_; }
      set {
        gender_ = value;
      }
    }

    /// <summary>Field number for the "phone_num" field.</summary>
    public const int PhoneNumFieldNumber = 5;
    private ulong phoneNum_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ulong PhoneNum {
      get { return phoneNum_; }
      set {
        phoneNum_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as personal_info);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(personal_info other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Id != other.Id) return false;
      if (Name != other.Name) return false;
      if (Age != other.Age) return false;
      if (Gender != other.Gender) return false;
      if (PhoneNum != other.PhoneNum) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Id != 0) hash ^= Id.GetHashCode();
      if (Name.Length != 0) hash ^= Name.GetHashCode();
      if (Age != 0) hash ^= Age.GetHashCode();
      if (Gender != global::Tom.gender_type.Male) hash ^= Gender.GetHashCode();
      if (PhoneNum != 0UL) hash ^= PhoneNum.GetHashCode();
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Id != 0) {
        output.WriteRawTag(8);
        output.WriteUInt32(Id);
      }
      if (Name.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(Name);
      }
      if (Age != 0) {
        output.WriteRawTag(24);
        output.WriteUInt32(Age);
      }
      if (Gender != global::Tom.gender_type.Male) {
        output.WriteRawTag(32);
        output.WriteEnum((int) Gender);
      }
      if (PhoneNum != 0UL) {
        output.WriteRawTag(40);
        output.WriteUInt64(PhoneNum);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Id != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Id);
      }
      if (Name.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Name);
      }
      if (Age != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Age);
      }
      if (Gender != global::Tom.gender_type.Male) {
        size += 1 + pb::CodedOutputStream.ComputeEnumSize((int) Gender);
      }
      if (PhoneNum != 0UL) {
        size += 1 + pb::CodedOutputStream.ComputeUInt64Size(PhoneNum);
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(personal_info other) {
      if (other == null) {
        return;
      }
      if (other.Id != 0) {
        Id = other.Id;
      }
      if (other.Name.Length != 0) {
        Name = other.Name;
      }
      if (other.Age != 0) {
        Age = other.Age;
      }
      if (other.Gender != global::Tom.gender_type.Male) {
        Gender = other.Gender;
      }
      if (other.PhoneNum != 0UL) {
        PhoneNum = other.PhoneNum;
      }
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
          case 8: {
            Id = input.ReadUInt32();
            break;
          }
          case 18: {
            Name = input.ReadString();
            break;
          }
          case 24: {
            Age = input.ReadUInt32();
            break;
          }
          case 32: {
            Gender = (global::Tom.gender_type) input.ReadEnum();
            break;
          }
          case 40: {
            PhoneNum = input.ReadUInt64();
            break;
          }
        }
      }
    }

  }

  public sealed partial class personal_info_list : pb::IMessage<personal_info_list> {
    private static readonly pb::MessageParser<personal_info_list> _parser = new pb::MessageParser<personal_info_list>(() => new personal_info_list());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<personal_info_list> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Tom.TellistReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public personal_info_list() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public personal_info_list(personal_info_list other) : this() {
      info_ = other.info_.Clone();
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public personal_info_list Clone() {
      return new personal_info_list(this);
    }

    /// <summary>Field number for the "info" field.</summary>
    public const int InfoFieldNumber = 1;
    private static readonly pb::FieldCodec<global::Tom.personal_info> _repeated_info_codec
        = pb::FieldCodec.ForMessage(10, global::Tom.personal_info.Parser);
    private readonly pbc::RepeatedField<global::Tom.personal_info> info_ = new pbc::RepeatedField<global::Tom.personal_info>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<global::Tom.personal_info> Info {
      get { return info_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as personal_info_list);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(personal_info_list other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if(!info_.Equals(other.info_)) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      hash ^= info_.GetHashCode();
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      info_.WriteTo(output, _repeated_info_codec);
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      size += info_.CalculateSize(_repeated_info_codec);
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(personal_info_list other) {
      if (other == null) {
        return;
      }
      info_.Add(other.info_);
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
          case 10: {
            info_.AddEntriesFrom(input, _repeated_info_codec);
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
