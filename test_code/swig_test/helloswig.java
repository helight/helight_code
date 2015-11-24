public class helloswig {

    static {
        System.out.println(System.getProperty("java.library.path"));
        System.loadLibrary("shape");
    }

    public static void main(String argv[]) {
        //
        System.out.println( "Creating some objects:" );
        Circle c = new Circle(10);
        System.out.println( "    Created circle " + c );
        Square s = new Square(10);
        System.out.println( "    Created square " + s );

        c.delete();
        s.delete();

        System.out.println( "Goodbye" );
    }
}
