package org.zhwen.sparkproject;

import org.apache.spark.Partitioner;

import org.zhwen.cityhash.cityhashJNI;

public class CityHashPartitioner extends Partitioner {
	static {                                                                                        
        System.out.println("xxxxxxxxxxxxxxxxxxxx" + System.getProperty("java.library.path"));                                
        System.loadLibrary("cityhash");                                                                
    } 
	private static final long serialVersionUID = 1L;
	private int numParts; 

	
	public CityHashPartitioner(int numParts) {
		this.numParts = numParts;
	}

	public int getPartition(Object key) {
		return (int)cityhashJNI.CityHash64Mod(key.toString(), key.toString().length(), numParts);
	}
	
	public int numPartitions() {
		return numParts;
	}
}
/*
     public static class RandomPartitioner extends Partitioner {
        private int numParts;
        private Random rnd = new Random();

        public RandomPartitioner(int numParts) {
            this.numParts = numParts;
        }

        public int numPartitions() {
            return numParts;
        }

        public int getPartition(Object key) {
            return rnd.nextInt(Integer.MAX_VALUE) % numPartitions();
        }

        public boolean equals(Object other) {
            if (other == this) {
                return true;
            }
            if (!(other instanceof RandomPartitioner)) {
                return false;
            }
            return ((RandomPartitioner) other).numPartitions() == numPartitions();
        }
    }
 */