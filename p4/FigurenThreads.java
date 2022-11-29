public class FigurenThreads {
    public static void main(String[] args) {
        var figur = new AndereFigur();
        var reader = new Leser(figur);
        var writer = new Schreiber(figur);

        writer.setDaemon(true);
        reader.start();
        writer.start();
    }
}
