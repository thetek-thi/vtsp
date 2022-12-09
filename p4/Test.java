public class Test {
    public static void main(String[] args) {
        try {
            var id = new UniqueId("id.dat");
            id.init(10000);
            for (var i = 0; i < 5; i++) {
                var thread = new TestThread(i, id);
                thread.start();
            }
        } catch (Exception e) { }
    }

    static class TestThread extends Thread {
        int name;
        UniqueId id;

        public TestThread(int name, UniqueId id) {
            this.name = name;
            this.id = id;
        }

        public void run() {
            for (var i = 0; i < 10; i++) {
                System.out.print(""); // required because otherwise the threads don't get "distributed" properly
                synchronized (System.out) { // for correct output order
                    try {
                        var next = id.getNext();
                        System.out.println("thread " + name + ": " + next);
                    } catch (Exception e) { }
                }
            }
        }
    }
}
