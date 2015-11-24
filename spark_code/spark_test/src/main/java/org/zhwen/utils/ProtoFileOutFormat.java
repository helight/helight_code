package org.zhwen.utils;

import java.io.DataOutputStream;
import java.io.IOException;


import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapred.FileOutputFormat;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.RecordWriter;
import org.apache.hadoop.util.Progressable;
import org.zhwen.utils.Bigtable.BigtableData;


public class ProtoFileOutFormat<K, V> extends FileOutputFormat<K, V> {

	@Override
	public RecordWriter<K, V> getRecordWriter(FileSystem filesystem, JobConf job, String name, Progressable progress)
			throws IOException {
		// TODO Auto-generated method stub

		Path file = (Path) FileOutputFormat.getTaskOutputPath(job, name);

		FileSystem fs = file.getFileSystem(job);

		FSDataOutputStream fileOut = ((FileSystem) fs).create(file, progress);
		// file, progress);
		// @todo write file head
		
		// return new LineRecordWrite(new DataOutputStream(out));
		return (RecordWriter<K, V>) new ProtoFileRecordWriter(fileOut);
	}

	protected class ProtoFileRecordWriter<K, V> implements RecordWriter<K, V> {

		protected DataOutputStream out;
		private final byte[] keyValueSeparator;

		public ProtoFileRecordWriter(DataOutputStream out) {
			this(out,"");
		}

		public ProtoFileRecordWriter(DataOutputStream out, String keyValueSeparator) {
			this.out = out;
			this.keyValueSeparator = keyValueSeparator.getBytes();
		}

		private void  writeObject(Object o) throws IOException {
			// this.out.write(o.toString().getBytes());
			((BigtableData) o).writeTo(this.out);
			
		}
		public void write(K text, V value) throws IOException {
			if (value != null) {
				writeObject(value);
			}
		}

		@Override
		public void close(org.apache.hadoop.mapred.Reporter arg0) throws IOException {
			// TODO Auto-generated method stub
			out.close();
			
		}
	}
}
/*
public class ProtoFileOutFormat<K, V> extends FileOutputFormat<K, V> {
	@Override
	public RecordWriter<K, V> getRecordWriter(TaskAttemptContext taskAttemptContext)
			throws IOException, InterruptedException {
		Configuration configuration = taskAttemptContext.getConfiguration();
		Path path = getDefaultWorkFile(taskAttemptContext, "");
		FileSystem fileSystem = path.getFileSystem(configuration);
		FSDataOutputStream out = fileSystem.create(path, false);
		return new ImageFileRecordWriter(out);
	}

	
}
*/