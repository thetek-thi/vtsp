import java.rmi.Naming;
import java.time.Instant;

public class DaytimeClient {
  public static void main(String args[]) throws Exception {
    String host = args[0];

    System.out.println("(before request) " + Instant.now().toString());
    Daytime remote = (Daytime) Naming.lookup("//" + host + "/daytime");
    String recvd = remote.getDaytime();
    System.out.println("(from request)   " + recvd);
    System.out.println("(after request)  " + Instant.now().toString());
  }
}
