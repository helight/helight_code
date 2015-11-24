package org.zhwen.sparkproject;

import scala.Tuple2;
import scala.io.BytePickle.PU;

import org.apache.hadoop.classification.InterfaceAudience.Public;
import org.apache.hadoop.hdfs.server.namenode.web.resources.NamenodeWebHdfsMethods;
import org.apache.spark.HashPartitioner;
import org.apache.spark.Partition;
import org.apache.spark.Partitioner;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.PairFunction;
import org.zhwen.utils.ProtoFileOutFormat;
import org.zhwen.utils.Bigtable.BigtableData;

public class WordCount {    
	
	public static void main(String[] args) throws Exception {

		SparkConf sparkConf = new SparkConf().setAppName("JavaWordCount");
		JavaSparkContext ctx = new JavaSparkContext(sparkConf);
		JavaRDD<String> lines = ctx.textFile("/data/spark/input/test2.data");

		JavaPairRDD<String, String> records = lines.mapToPair(new PairFunction<String, String, String>() {
			@Override
			public Tuple2<String, String> call(String record) {
				String[] arr = record.split("\\|");
				return new Tuple2<String, String>(arr[2], record);
			}
		});

		records.partitionBy(new HashPartitioner(10)).saveAsTextFile("/data/spark/output/hash_out/");

/*		JavaRDD<BigtableData> pbrecords = lines.map(new CreateBigtableData()).filter(new Function<BigtableData, Boolean>() {
			
			@Override
			public Boolean call(BigtableData arg0) throws Exception {
				// TODO Auto-generated method stub
				return arg0 != null;
			}
		});

		pbrecords.saveAsObjectFile("/data/spark/output/rdd_pb_out/");	*/	

		JavaPairRDD<String, BigtableData> pb_key_records = lines.mapToPair(new PairFunction<String, String, BigtableData>() {
			@Override
			public Tuple2<String, BigtableData> call(String record) {
				String[] arr = record.split("\\|");
				
				try {
				BigtableData.Builder bdrecord = BigtableData.newBuilder();
				bdrecord.setPrimaryKey(arr[2]);

				int arr_len = arr.length;
				for(int i = 4; i < arr_len; ++i) {
					bdrecord.addLevelValues(Integer.parseInt(arr[i]));
				}
				
				return new Tuple2<String, BigtableData>(arr[2], bdrecord.build());
				} catch (Exception e) {
					e.printStackTrace();
					return null;
				}
			}
		});
		CityHashPartitioner partitioner = new CityHashPartitioner(10);
		pb_key_records.repartitionAndSortWithinPartitions(partitioner);
			//.saveAsHadoopFile("/data/spark/output/pb_val_out/", String.class, String.class, PbOutPutFormatClass.class);
		
		JavaPairRDD<String, String> test_records = lines.mapToPair(new PairFunction<String, String, String>() {
			@Override
			public Tuple2<String, String> call(String record) {
				String[] arr = record.split("\\|");
				return new Tuple2<String, String>(arr[2], arr[4]);
			}
		});
	
		
		test_records.repartitionAndSortWithinPartitions(partitioner).saveAsTextFile("/data/spark/output/test_out/");	
		     // records.partitionBy(new HashPartitioner(10)).saveAsTextFile("/data/spark/output/wc_out/");
		ctx.stop();
	}
}