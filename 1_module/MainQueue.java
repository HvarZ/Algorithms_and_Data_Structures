import java.util.Scanner;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.Writer;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.FileNotFoundException;
import java.io.IOException;

public class MainQueue {
    public static class Queue {
        private String[] data_;
        private int capacity_;
        private int front_;
        private int tail_;

        public Queue(int capacity) {
            data_ = new String[capacity];
            capacity_ = capacity;
            front_ = tail_ = -1;
        }

        public void push(String value) throws Exception {
            if ((front_ == 0 && tail_ == capacity_ - 1) || (front_ == tail_ + 1)) {
                throw new Exception("overflow");
            }
            if (front_ == -1) {
                front_++;
            }
            tail_ = (tail_ + 1) % capacity_;
            data_[tail_] = value;
        }

        public String pop() throws Exception {
            if (front_ == -1) {
                throw new Exception("underflow");
            }
            String result = data_[front_];

            if (front_ == tail_) {
                front_ = -1;
                tail_ = -1;
            } else {
                front_ = (front_ + 1) % capacity_;
            }
            return result;
        }

        public void print(Writer writer) throws Exception {
            if (front_ == -1) {
                throw new Exception("empty");
            }
            for (int i = front_; i != tail_; i = (i + 1) % capacity_) {
                writer.write(data_[i] + " ");
            }
            writer.write(data_[tail_] + "\n");
        }
    }

    public static void main(String[] args) {
        String regexSet = "^set_size\\s[0-9][0-9]*$";
        String regexCommand = "^push\\s\\S*$|^pop$|^print$|";
        String buffer;
        Queue queue = null;
        FileInputStream readerStream = null;
        FileOutputStream writerStream = null;
        try {
            try {
                readerStream = new FileInputStream(args[0]);
                writerStream = new FileOutputStream(args[1]);
            } catch (FileNotFoundException ex) {
                System.err.println(ex.getMessage());
            }

            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(writerStream));

            Scanner input = new Scanner(readerStream);
            while (input.hasNext()) {
                buffer = input.nextLine();
                if (buffer.length() == 0) {
                    continue;
                } else if (buffer.matches(regexSet)) {
                    String[] splitCommand = buffer.split(" ");
                    int sizeStack = Integer.parseInt(splitCommand[1]);
                    queue = new Queue(sizeStack);
                    break;
                } else {
                    writer.write("error" + "\n");
                }
            }
            while (input.hasNext()) {
                buffer = input.nextLine();
                if (buffer.length() == 0) {
                    continue;
                } else if (buffer.matches(regexCommand)) {
                    String[] splitCommand = buffer.split(" ");
                    if (splitCommand[0].equals("push")) {
                        try {
                            queue.push(splitCommand[1]);
                        } catch (Exception ex) {
                            writer.write("overflow" + "\n");
                        }
                    } else if (splitCommand[0].equals("pop")) {
                        try {
                            writer.write(queue.pop() + "\n");
                        } catch (Exception ex) {
                            writer.write("underflow" + "\n");
                        }
                    } else if (splitCommand[0].equals("print")) {
                        try {
                            queue.print(writer);
                        } catch (Exception ex) {
                            writer.write("empty" + "\n");
                        }
                    } else {
                        writer.write("error" + "\n");
                    }
                } else {
                    writer.write("error" + "\n");
                }
            }
            writer.flush();
        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }
}
