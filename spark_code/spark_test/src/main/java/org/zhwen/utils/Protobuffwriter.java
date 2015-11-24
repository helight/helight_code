package org.zhwen.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.security.Timestamp;
import java.sql.Struct;

import org.zhwen.utils.Bigtable.BigtableData;

import com.google.protobuf.Message;
import com.sun.tools.internal.jxc.ap.Const;

public class Protobuffwriter {

	private static int PbFileHeaderMagicNumber = 621;
	private static int PbFileHeaderBlockSize = 111; // 100*(1<<20);

	public static class ProtoWriter {
		private FileOutputStream out = null;

		public byte[] intobyte(int number) {

			int temp = number;

			byte[] b = new byte[4];

			for (int i = b.length - 1; i > -1; --i) {
				b[i] = new Integer(temp & 0xff).byteValue();
				temp = temp >> 8;
			}

			return b;
		}

		public byte[] intToByteArray(int i) {
			byte[] result = new byte[4];
			result[0] = (byte) ((i >> 24) & 0xFF);
			result[1] = (byte) ((i >> 16) & 0xFF);
			result[2] = (byte) ((i >> 8) & 0xFF);
			result[3] = (byte) (i & 0xFF);
			return result;
		}

		public boolean openFile(String path) {
			// long newtime = System.currentTimeMillis();
			try {
				Integer tt = PbFileHeaderMagicNumber;
				tt.toString().getBytes();
				out = new FileOutputStream(new File(path));
				out.write(intToByteArray(PbFileHeaderMagicNumber), 0, 4);
				out.write(intToByteArray(PbFileHeaderBlockSize), 0, 4);
				out.write(intToByteArray(PbFileHeaderMagicNumber), 0, 4);
				out.write(intToByteArray(PbFileHeaderBlockSize), 0, 4);
			} catch (Exception e) {
				e.printStackTrace();
				return false;
			}
			return true;
		}

		public int writeObject(BigtableData o) {
			int size = o.toByteArray().length;
			try {
				out.write(size);
				out.write(o.toByteArray());
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return 0;
			}
			return size;
		}

		public void close() {
			try {
				if (out != null)
					out.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static class ProtoReader {
		private FileInputStream in = null;

		public int bytetoint(byte[] b) {
			int t = 0;
			int len = b.length > 4 ? 4 : b.length;
			byte bLoop;
			for (int i = 0; i < len; ++i) {
				bLoop = b[i];
				t += (bLoop & 0xFF) << (8 * i);
			}
			return t;
		}

		public boolean openFile(String path) {
			try {
				in = new FileInputStream(new File(path));
				byte[] b = new byte[16];
				in.read(b, 0, 16);
				int magic_number = bytetoint(b);
				System.out.println("magic_number: " + magic_number);
			} catch (Exception e) {
				e.printStackTrace();
				return false;
			}
			return true;
		}

		public BigtableData readObject() {
			int size = 0;
			try {
				size = in.read();
				System.out.println("size: " + size);
				byte[] tempchars = new byte[size];
				in.read(tempchars);
				BigtableData tt = BigtableData.parseFrom(tempchars);
				return tt;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return null;
		}

		public void close() {
			try {
				if (in != null)
					in.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {

		String path = "/data/test/test.pb";
		ProtoWriter pwriter = new ProtoWriter();
		pwriter.openFile(path);

		// TODO Auto-generated method stub
		BigtableData.Builder data = BigtableData.newBuilder();
		data.setPrimaryKey("ttt");
		data.addLevelValues(12);
		data.addLevelValues(123);
		data.addLevelValues(124);
		data.addLevelValues(125);
		data.addLevelValues(126);
		pwriter.writeObject(data.build());

		data = BigtableData.newBuilder();
		data.setPrimaryKey("kkk");
		data.addLevelValues(12);
		data.addLevelValues(123);
		pwriter.writeObject(data.build());
		pwriter.close();

		ProtoReader preader = new ProtoReader();
		preader.openFile(path);
		BigtableData tt = preader.readObject();
		System.out.println("key: " + tt.getPrimaryKey() + " value: " + tt.getLevelValuesCount());
		tt = preader.readObject();
		System.out.println("key: " + tt.getPrimaryKey() + " value: " + tt.getLevelValuesCount());
		preader.close();
	}

}
