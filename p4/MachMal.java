public class MachMal {
  public static void eineZehntelSekundeLangGarNichts() {
    try {
      Thread.sleep(100);
    } 
    catch (InterruptedException e) {
    }
  }

  public static void eineSekundeLangGarNichts() {
    try {
      Thread.sleep(1000);
    } 
    catch (InterruptedException e) {
    }
  }
}
