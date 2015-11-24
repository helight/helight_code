namespace cpp xlight

// enum EnOpType {
//     CMD_OK = 0,
//     CMD_NOT_EXIT = 2000,
//     CMD_EXIT = 2001,
//     CMD_ADD = 2002
// }

struct StUser {
    1: required i32 userId;
    2: optional string userName;
    4: string language;
}

service SeTest {
//     bool AddUser(1: StUser user);
    StUser SearchUser(1:string name);
    oneway void NopNop()
}
