
import org.apache.hadoop.conf.Configuration
import org.apache.hadoop.fs.FileSystem
import org.apache.hadoop.fs.FileUtil
import org.apache.hadoop.fs.Path
import org.apache.spark._


object SparkScala {
  def main(args: Array[String]): Unit = {
    println("hello Spark Scala Test ...")    
    
    var hdfsConf = new Configuration();
    val sparkConf = new SparkConf().setAppName("Spark Scala Test")
    sparkConf.set("spark.eventLog.enabled", "true");
		sparkConf.set("spark.port.maxRetries", "128");
		sparkConf.set("spark.task.maxFailures", "16");
    val sc = new SparkContext(sparkConf)
    
    
    
/*    sc.hadoopConfiguration().set("dfs.blocksize", "33554432");
		sc.hadoopConfiguration().set("dfs.nameservices", "ispark-01");
		sc.hadoopConfiguration().set("dfs.ha.namenodes.ispark-01", "n1,n2");
		sc.hadoopConfiguration().set("dfs.namenode.rpc-address.ispark-01.n1", "10.111.84.20:8020");
		sc.hadoopConfiguration().set("dfs.namenode.rpc-address.ispark-01.n2", "10.111.84.88:8020");
		sc.hadoopConfiguration().set("dfs.client.failover.proxy.provider.ispark-01",
				"org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider");
		sc.hadoopConfiguration().set("fs.defaultFS", "hdfs://10.111.84.20:8020");*/

		hdfsConf.set("fs.default.name", "hdfs://10.111.84.20:8020");
		var hdfs = FileSystem.get(hdfsConf);
		
    println("finish test ...")
    sc.stop();
  }
}