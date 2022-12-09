import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.time.Instant;

public class DaytimeImpl
  extends UnicastRemoteObject
  implements Daytime
{
  public DaytimeImpl() throws RemoteException { }

  public String getDaytime() {
    return Instant.now().toString();
  }
}
