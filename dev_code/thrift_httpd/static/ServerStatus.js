//
// Autogenerated by Thrift Compiler (0.9.0)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//


//HELPER FUNCTIONS AND STRUCTURES

ServerStatus_ping_args = function(args) {
};
ServerStatus_ping_args.prototype = {};
ServerStatus_ping_args.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    input.skip(ftype);
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_ping_args.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_ping_args');
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_ping_result = function(args) {
};
ServerStatus_ping_result.prototype = {};
ServerStatus_ping_result.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    input.skip(ftype);
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_ping_result.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_ping_result');
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_add_args = function(args) {
  this.num1 = null;
  this.num2 = null;
  if (args) {
    if (args.num1 !== undefined) {
      this.num1 = args.num1;
    }
    if (args.num2 !== undefined) {
      this.num2 = args.num2;
    }
  }
};
ServerStatus_add_args.prototype = {};
ServerStatus_add_args.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.I32) {
        this.num1 = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 2:
      if (ftype == Thrift.Type.I32) {
        this.num2 = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_add_args.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_add_args');
  if (this.num1 !== null && this.num1 !== undefined) {
    output.writeFieldBegin('num1', Thrift.Type.I32, 1);
    output.writeI32(this.num1);
    output.writeFieldEnd();
  }
  if (this.num2 !== null && this.num2 !== undefined) {
    output.writeFieldBegin('num2', Thrift.Type.I32, 2);
    output.writeI32(this.num2);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_add_result = function(args) {
  this.success = null;
  if (args) {
    if (args.success !== undefined) {
      this.success = args.success;
    }
  }
};
ServerStatus_add_result.prototype = {};
ServerStatus_add_result.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
      if (ftype == Thrift.Type.I32) {
        this.success = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_add_result.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_add_result');
  if (this.success !== null && this.success !== undefined) {
    output.writeFieldBegin('success', Thrift.Type.I32, 0);
    output.writeI32(this.success);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_calculate_args = function(args) {
  this.logid = null;
  this.w = null;
  if (args) {
    if (args.logid !== undefined) {
      this.logid = args.logid;
    }
    if (args.w !== undefined) {
      this.w = args.w;
    }
  }
};
ServerStatus_calculate_args.prototype = {};
ServerStatus_calculate_args.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.I32) {
        this.logid = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 2:
      if (ftype == Thrift.Type.STRUCT) {
        this.w = new Work();
        this.w.read(input);
      } else {
        input.skip(ftype);
      }
      break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_calculate_args.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_calculate_args');
  if (this.logid !== null && this.logid !== undefined) {
    output.writeFieldBegin('logid', Thrift.Type.I32, 1);
    output.writeI32(this.logid);
    output.writeFieldEnd();
  }
  if (this.w !== null && this.w !== undefined) {
    output.writeFieldBegin('w', Thrift.Type.STRUCT, 2);
    this.w.write(output);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_calculate_result = function(args) {
  this.success = null;
  this.ouch = null;
  if (args instanceof InvalidOperation) {
    this.ouch = args;
    return;
  }
  if (args) {
    if (args.success !== undefined) {
      this.success = args.success;
    }
    if (args.ouch !== undefined) {
      this.ouch = args.ouch;
    }
  }
};
ServerStatus_calculate_result.prototype = {};
ServerStatus_calculate_result.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
      if (ftype == Thrift.Type.I32) {
        this.success = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 1:
      if (ftype == Thrift.Type.STRUCT) {
        this.ouch = new InvalidOperation();
        this.ouch.read(input);
      } else {
        input.skip(ftype);
      }
      break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_calculate_result.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_calculate_result');
  if (this.success !== null && this.success !== undefined) {
    output.writeFieldBegin('success', Thrift.Type.I32, 0);
    output.writeI32(this.success);
    output.writeFieldEnd();
  }
  if (this.ouch !== null && this.ouch !== undefined) {
    output.writeFieldBegin('ouch', Thrift.Type.STRUCT, 1);
    this.ouch.write(output);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_getStruct_args = function(args) {
  this.key = null;
  if (args) {
    if (args.key !== undefined) {
      this.key = args.key;
    }
  }
};
ServerStatus_getStruct_args.prototype = {};
ServerStatus_getStruct_args.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.I32) {
        this.key = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_getStruct_args.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_getStruct_args');
  if (this.key !== null && this.key !== undefined) {
    output.writeFieldBegin('key', Thrift.Type.I32, 1);
    output.writeI32(this.key);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatus_getStruct_result = function(args) {
  this.success = null;
  if (args) {
    if (args.success !== undefined) {
      this.success = args.success;
    }
  }
};
ServerStatus_getStruct_result.prototype = {};
ServerStatus_getStruct_result.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
      if (ftype == Thrift.Type.STRUCT) {
        this.success = new SharedStruct();
        this.success.read(input);
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

ServerStatus_getStruct_result.prototype.write = function(output) {
  output.writeStructBegin('ServerStatus_getStruct_result');
  if (this.success !== null && this.success !== undefined) {
    output.writeFieldBegin('success', Thrift.Type.STRUCT, 0);
    this.success.write(output);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ServerStatusClient = function(input, output) {
    this.input = input;
    this.output = (!output) ? input : output;
    this.seqid = 0;
};
ServerStatusClient.prototype = {};
ServerStatusClient.prototype.ping = function() {
  this.send_ping();
  this.recv_ping();
};

ServerStatusClient.prototype.send_ping = function() {
  this.output.writeMessageBegin('ping', Thrift.MessageType.CALL, this.seqid);
  var args = new ServerStatus_ping_args();
  args.write(this.output);
  this.output.writeMessageEnd();
  return this.output.getTransport().flush();
};

ServerStatusClient.prototype.recv_ping = function() {
  var ret = this.input.readMessageBegin();
  var fname = ret.fname;
  var mtype = ret.mtype;
  var rseqid = ret.rseqid;
  if (mtype == Thrift.MessageType.EXCEPTION) {
    var x = new Thrift.TApplicationException();
    x.read(this.input);
    this.input.readMessageEnd();
    throw x;
  }
  var result = new ServerStatus_ping_result();
  result.read(this.input);
  this.input.readMessageEnd();

  return;
};
ServerStatusClient.prototype.add = function(num1, num2) {
  this.send_add(num1, num2);
  return this.recv_add();
};

ServerStatusClient.prototype.send_add = function(num1, num2) {
  this.output.writeMessageBegin('add', Thrift.MessageType.CALL, this.seqid);
  var args = new ServerStatus_add_args();
  args.num1 = num1;
  args.num2 = num2;
  args.write(this.output);
  this.output.writeMessageEnd();
  return this.output.getTransport().flush();
};

ServerStatusClient.prototype.recv_add = function() {
  var ret = this.input.readMessageBegin();
  var fname = ret.fname;
  var mtype = ret.mtype;
  var rseqid = ret.rseqid;
  if (mtype == Thrift.MessageType.EXCEPTION) {
    var x = new Thrift.TApplicationException();
    x.read(this.input);
    this.input.readMessageEnd();
    throw x;
  }
  var result = new ServerStatus_add_result();
  result.read(this.input);
  this.input.readMessageEnd();

  if (null !== result.success) {
    return result.success;
  }
  throw 'add failed: unknown result';
};
ServerStatusClient.prototype.calculate = function(logid, w) {
  this.send_calculate(logid, w);
  return this.recv_calculate();
};

ServerStatusClient.prototype.send_calculate = function(logid, w) {
  this.output.writeMessageBegin('calculate', Thrift.MessageType.CALL, this.seqid);
  var args = new ServerStatus_calculate_args();
  args.logid = logid;
  args.w = w;
  args.write(this.output);
  this.output.writeMessageEnd();
  return this.output.getTransport().flush();
};

ServerStatusClient.prototype.recv_calculate = function() {
  var ret = this.input.readMessageBegin();
  var fname = ret.fname;
  var mtype = ret.mtype;
  var rseqid = ret.rseqid;
  if (mtype == Thrift.MessageType.EXCEPTION) {
    var x = new Thrift.TApplicationException();
    x.read(this.input);
    this.input.readMessageEnd();
    throw x;
  }
  var result = new ServerStatus_calculate_result();
  result.read(this.input);
  this.input.readMessageEnd();

  if (null !== result.ouch) {
    throw result.ouch;
  }
  if (null !== result.success) {
    return result.success;
  }
  throw 'calculate failed: unknown result';
};
ServerStatusClient.prototype.getStruct = function(key) {
  this.send_getStruct(key);
  return this.recv_getStruct();
};

ServerStatusClient.prototype.send_getStruct = function(key) {
  this.output.writeMessageBegin('getStruct', Thrift.MessageType.CALL, this.seqid);
  var args = new ServerStatus_getStruct_args();
  args.key = key;
  args.write(this.output);
  this.output.writeMessageEnd();
  return this.output.getTransport().flush();
};

ServerStatusClient.prototype.recv_getStruct = function() {
  var ret = this.input.readMessageBegin();
  var fname = ret.fname;
  var mtype = ret.mtype;
  var rseqid = ret.rseqid;
  if (mtype == Thrift.MessageType.EXCEPTION) {
    var x = new Thrift.TApplicationException();
    x.read(this.input);
    this.input.readMessageEnd();
    throw x;
  }
  var result = new ServerStatus_getStruct_result();
  result.read(this.input);
  this.input.readMessageEnd();

  if (null !== result.success) {
    return result.success;
  }
  throw 'getStruct failed: unknown result';
};
