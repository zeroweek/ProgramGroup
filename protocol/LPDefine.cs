//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: LPDefine.proto
namespace LPDefine
{
    [global::ProtoBuf.ProtoContract(Name=@"e_MsgID")]
    public enum e_MsgID
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"msg_begin", Value=1)]
      msg_begin = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"msg_req_server_register", Value=1001)]
      msg_req_server_register = 1001,
            
      [global::ProtoBuf.ProtoEnum(Name=@"msg_ack_server_register", Value=1002)]
      msg_ack_server_register = 1002,
            
      [global::ProtoBuf.ProtoEnum(Name=@"msg_req_client_login", Value=2001)]
      msg_req_client_login = 2001,
            
      [global::ProtoBuf.ProtoEnum(Name=@"msg_ack_client_login", Value=2002)]
      msg_ack_client_login = 2002,
            
      [global::ProtoBuf.ProtoEnum(Name=@"msg_end", Value=65535)]
      msg_end = 65535
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"e_ResultCode")]
    public enum e_ResultCode
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"eRstUnknow", Value=0)]
      eRstUnknow = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eRstSuccess", Value=1)]
      eRstSuccess = 1
    }
  
}