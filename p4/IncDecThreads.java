// File  : IncDecThreads.java
// Date  : 9.8.2005 23:15:35

// Diese Klasse definiert einen Zähler,
// der von einem Thread um die Größe increment
// weitergeschaltet wird.
// In der Anwendung wird als increment nur +1 bzw. -1 benutzt.
// Das Programm entstand in Kooperation mit
// Herrn Dipl. Inf. (FH) Martin Opel

// Ab JDK 1.5

public class IncDecThreads extends Thread {
  // Maximim für die Iteration
  public static final long MAX = 10000000;
  // Um die folgende Größe weiterschalten
  private long increment = 0;
  
  // Festlegung des Wertes von increment
  public IncDecThreads (long increment) {
    this.increment = increment;
  }
  
  // Erzeuge die Threads, Ablauf, Warten auf die Threads
  public static void main (String[] args) {
    IncDecThreads thread1 = new IncDecThreads(+1);
    IncDecThreads thread2 = new IncDecThreads(-1);
    long start = System.currentTimeMillis ();
    thread1.start(); 
    thread2.start();
    try {
      thread1.join ();
      thread2.join ();
    } catch (Exception e) {
    }
    System.out.println ("zaehler: "+  zaehler + " nach msec: "+(System.currentTimeMillis ()-start));
  }

  
  // Dies ist der Zähler, der von diversen Threads
  // bearbeitet wird.
  static long zaehler=0;
  
  

  // Unsynchronisiert geht es schnell, aber....
  public void demoUnSync () {
    for (long i = 0; i < MAX; i++) {
      zaehler = zaehler + increment;
    }
  }

  // Synchronized!
  public void demoSync () {
    for (long i = 0; i < MAX; i++) {
      synchronized (System.out) { // didnt know what other class to use ¯\_(·_·)_/¯
        zaehler = zaehler + increment;
      }
    }
  }

  // Einfach rauf oder runter zählen...
  public void demo () {
    for (long i = 0; i < 100; i++) {
      zaehler = zaehler + increment;
      try {
        sleep ((int)(1000*Math.random ()));
      } catch (Exception e) {
        return;
      }
    }
  }
  
  public void run() {
    //demoUnSync ();  // Unsynchronisiertes Zählen
    demoSync ();  // Synchronisiertes Zählen
  }
}
