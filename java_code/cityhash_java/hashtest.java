
public class hashtest
{
    static {
        System.out.println(System.getProperty("java.library.path"));
        System.loadLibrary("cityhash");
    }

    public static void modtest(String tt) {
        long code = (cityhashJNI.CityHash64IdataMod(tt, tt.length(), 10));
        System.out.println("code: " + code + " length: " + tt.length());
    }


    public static void main( String[] args )
    {

        System.out.println( "Hello World!" );
        String tt = "hello world";

        // UnsignedLong code = UnsignedLong.asUnsigned(cityhash.hashBytes(tt.getBytes(), 0, tt.length()).asLong());
        // Long code = Math.abs(cityhash1.hashBytes(tt.getBytes(), 0, tt.length()).asLong());
        // System.out.println("code: " + code);
        // code = Math.abs(hashtt.cityHash64(tt.getBytes(), 0, tt.length()));
        // System.out.println("code2: " + code);

        modtest(tt);
        modtest("hello");
        modtest("world");
    }
}
