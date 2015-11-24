package org.zhwen.sparkproject;

import java.io.Serializable;
import java.util.ArrayList;

import javax.xml.crypto.Data;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.sql.DataFrame;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.RowFactory;
import org.apache.spark.sql.SQLContext;
import org.apache.spark.sql.functions;
import org.apache.spark.sql.catalyst.plans.logical.CreateTableAsSelect;
import org.apache.spark.sql.types.DataType;
import org.apache.spark.sql.types.DataTypes;
import org.apache.spark.sql.types.StructField;
import org.apache.spark.sql.types.StructType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sun.tools.javac.util.List;

import gnu.getopt.Getopt;
import parquet.hadoop.api.InitContext;

public class SqlTask implements Serializable {
	private static Logger logger = LoggerFactory.getLogger(SqlTask.class);

	private SQLContext sqlcontext;
	private transient JavaSparkContext sc = null;

	private String name;

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	private void init() {
		SparkConf sparkconf = new SparkConf().setAppName("SqlTask");
		sc = new JavaSparkContext(sparkconf);
		sqlcontext = new SQLContext(sc.sc());
	}

	private void sql_test1() {
		String infile_path = "/data/spark/input/test2.data";
		String tmpfile_path = "/data/spark/output/test2.tmp";
		String outfile_path = "/data/spark/output/test2.data";

		JavaRDD<String> data = sc.textFile(infile_path);

		logger.info("file path: " + infile_path);
		System.out.println("file path: " + infile_path);

		ArrayList<StructField> fields = new ArrayList<StructField>();
		// 20150908|20150906|1039280804|0|3|1|0|3|2178|46|0|114|-1|9

		fields.add(DataTypes.createStructField("date1", DataTypes.StringType, true)); // 1
		fields.add(DataTypes.createStructField("date2", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("uin", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data1", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data2", DataTypes.StringType, true)); // 5
		fields.add(DataTypes.createStructField("data3", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data4", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data5", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data6", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data7", DataTypes.StringType, true)); // 10
		fields.add(DataTypes.createStructField("data8", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data9", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data10", DataTypes.StringType, true));
		fields.add(DataTypes.createStructField("data11", DataTypes.StringType, true)); // 14

		StructType stschema = DataTypes.createStructType(fields);

		JavaRDD<Row> rowrdd = data.map(new Function<String, Row>() {

			private static final long serialVersionUID = 1L;

			public Row call(String record) throws Exception {
				String[] fields = record.split("\\|");
				ArrayList<Object> list = new ArrayList<Object>();
				for (String item : fields) {
					list.add(item);
				}
				return RowFactory.create(list.toArray());
			}
		});
		// Long lines = rowrdd.count();

		// logger.info("rowrdd: " + lines);
		// System.out.println("rowrdd: " + lines);

		DataFrame df = sqlcontext.createDataFrame(rowrdd, stschema);

		df.registerTempTable("datatest");
		df.printSchema();
		// DataFrame result2 = sqlcontext.sql("select * from datatest limit 5");
		// result2.show();
		DataFrame result = sqlcontext.sql("select date2, uin, data11 from datatest");
		// lines = result.count();
		// result.show();
		// logger.info("result: " + lines);
		// System.out.println("result: " + lines);

		// result.save(tmpfile_path);

		JavaRDD<String> dataret = result.javaRDD().map(new Function<Row, String>() {

			private static final long serialVersionUID = 1L;

			public String call(Row record) {
				StringBuffer sb = new StringBuffer();
				sb.append(record.get(0));
				for (int i = 1; i < record.length(); ++i) {
					sb.append("," + record.get(i));
				}
				// System.out.println("sb: " + sb.toString());
				return sb.toString();
			}
		});
		// lines = dataret.count();
		// logger.info("dataret: " + lines);
		// System.out.println("dataret: " + lines);

		dataret = dataret.coalesce(1);
		dataret.saveAsTextFile(outfile_path);

		logger.info("outfile_path: " + outfile_path);
		System.out.println("outfile_path: " + outfile_path);

	}

	private void close() {
		sc.close();
	}

	private boolean CreateTable(String tables, String se ) {
		logger.info("create tables: " + tables);
		// 1000015522_qq|tb_1000015522|PrimaryKey,string|,
		// 1000015573_qq_mix|tb_1000015573|PrimaryKey,string:gender,string:regtime,string:qqlevel,int|,
		String[] tableinfo = tables.split("|");
		String filename = tableinfo[0];
		String tableneme = tableinfo[1];
		String tablefield = tableinfo[2];
		// final String separt = se;
		// if (tableinfo.length > 3 && !tableinfo[3].equals(""))
		// 	separt = tableinfo[3];
		
		final ArrayList<String[]> listFields = new ArrayList<String[]>();
		for (String field : tablefield.split(":")) {
			listFields.add(field.replaceAll(" |\t", "").split(","));
		}
		// set up titles for table
		ArrayList<StructField> fields = new ArrayList<StructField>();
		try {
			for (String[] field : listFields) {
				DataType type = null;
				switch (field[1]) {
				case "string":
					type = DataTypes.StringType;
					break;
				case "int":
					type = DataTypes.IntegerType;
					break;
				case "long":
					type = DataTypes.LongType;
					break;
				case "float":
					type = DataTypes.FloatType;
					break;
				default:
					logger.error("error type: tables: " + field[1]);
					return false;
				}
				fields.add(DataTypes.createStructField(field[0], type, true));
			}
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		
		StructType stchema = DataTypes.createStructType(fields);
		
		// for mult files split by ","
		String[] filespath = filename.split(",");
		JavaRDD<String> idata = sc.emptyRDD();
		for (String ipath : filespath) {
			idata = idata.union(sc.textFile(ipath, 10));
		}
		
		JavaRDD<Row> rowRdd = idata.map(new Function<String, Row>() {
			public Row call(String record) {
				String[] item = record.split(",");
				ArrayList<Object> list = new ArrayList<Object>();
				
				int idx = 0;
				try {
					for (String[] fieldinfo : listFields) {
						if (item[idx].equals("nil")) {
							list.add(null);
						} else {
							switch (fieldinfo[1]) {
							case "string":
								list.add(fieldinfo[idx]);								
								break;
							case "int":
								list.add(Integer.parseInt(fieldinfo[idx]));
								break;
							case "long":
								list.add(Long.parseLong(fieldinfo[idx]));
								break;
							case "float":
								list.add(Float.parseFloat(fieldinfo[idx]));
								break;
							default:
								list.add(fieldinfo[idx]);
								break;
							}
						}
						++idx;
					}
					
				} catch (Exception e) {
					e.printStackTrace();
					return null;
				}
				return RowFactory.create(list.toArray());
			}
		}).filter(new Function<Row<Row, Boolean>() {
			@Override
			public Boolean call(Row arg0) {
				return arg0 != null;
			}
		});
		return true;
	}

	public void processsor(String tables, String sql, String output, String targetfield) {
		logger.info("processsor tables: " + tables + " sql: " + sql + " output: " + output + " targetfield: " + targetfield);
		init();

		if ("".equals(tables) || "".equals(sql) || "".equals(output) || "".equals(targetfield)) {
			logger.error("params error. tables: " + tables + " sql: " + sql + " output: " + output + " targetfield: "
					+ targetfield);
			sql_test1();
		} else {
			// 1000015522_qq:tb_1000015522:PrimaryKey,string:,;1000015573_qq_mix:tb_1000015573:PrimaryKey,string:gender,string:regtime,string:qqlevel,int:,
			String[] src_tables = tables.split(";");
			for (String table : src_tables) {
				CreateTable(table);
			}
		}

		close();
	}

	private static void showhelp() {
		System.out.println("SparkSqlTest tables sql targettablefield outputfile");
		System.out.println("================================");
		System.out.println("Usage: ied.idata.idata_task_project.DataExtractApp");
		System.out.println("");
		System.out.println("	-h	[optional] show help");
		System.out.println("	-p	[required ] task params, format: path|table name|field info(|separator)");
		System.out.println("	-s	[required ] task run sql");
		System.out.println("	-o	[required ] output file");
		System.out.println("	-d	[optional ] run in dev environment");
		System.out.println("	-f	[optional ] hadoop fs.default.name, default--hdfs://10.238.1.203:9000/");
		System.out.println("	-w	[optional ] overwrite the output file: default is not");
		System.out.println("	-b	[optional ] separator, default is blank");
		System.out.println("	-c	[optional ] target file fields, format:field1,int:field2,int");
		System.out.println("");
		System.out.println("================================");
	}

	public static void main(String[] args) {
		logger.info("SparkSql test Begin ...");		
		// 1000015522_qq:tb_1000015522:PrimaryKey,string:,;1000015573_qq_mix:tb_1000015573:PrimaryKey,string:gender,string:regtime,string:qqlevel,int:,
		String tables = "";
		// select
		// tb_1000015522.PrimaryKey,tb_1000015573.gender,tb_1000015573.regtime,tb_1000015573.qqlevel
		// from tb_1000015522
		// left outer join tb_1000015573 on
		// tb_1000015522.PrimaryKey=tb_1000015573.PrimaryKey order by
		// tb_1000015573.gender asc
		String sql = "";		
		String targetfield = ""; 	// PrimaryKey,string:gender,string:regtime,string:qqlevel,int
		String output = ""; 		// 10000015572
		String separator = ",";
		String hdfsname = "";
		boolean overWrite = false;

		/*if (args.length < 4) {
			showhelp();
			return;
		}*/

		// logger.info("args: args1: " + args[0] + " args2: " + args[1] + " args3: " + args[2] + " args4: " + args[3]);
		/*
		tables = args[0];
		sql = args[1];
		targetfield = args[2];
		output = args[3];		
		*/
	    Getopt opts = new Getopt("SqarkSql", args, "ht:o:s:f:wb:c:");	
		int c;
		while ((c = opts.getopt()) != -1) {
			switch (c) {
			case 'h':
				showhelp();
				break;
			case 't':
				tables = opts.getOptarg();
				break;
			case 'o':
				output = opts.getOptarg();
				break;
			case 's':
				sql = opts.getOptarg();
				break;
			case 'c':
				targetfield = opts.getOptarg();
				break;
			case 'f':
				hdfsname = opts.getOptarg();
				break;
			case 'w':
				overWrite = true;
				break;
			case 'b':
				separator = opts.getOptarg();
				if (separator.length() == 0) {
					showhelp();
				}
				break;			
			default:
				break;
			}
		}
		
		SqlTask sqltask = new SqlTask();
		logger.info("processsor tables: " + tables + " sql: " + sql + " output: " + output + " targetfield: " + targetfield);
		sqltask.processsor(tables, sql, output, targetfield);
	}
}
