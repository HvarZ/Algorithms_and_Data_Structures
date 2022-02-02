import java.util.BitSet;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.io.IOException;
import java.math.BigInteger;

public class MainBloomFilter {
    public static class BloomFilter {
        private BitSet bitset_;
        private int bitsetSize_;
        private int hashes_;
        private ArrayList<Integer> primeNumbers_;
        static final int MARCIN_31 = 2_147_483_647;
        static final int SIEVE_LIMIT = 11;

        private int hash(int index, BigInteger key) {
            return ((int)((key.remainder(new BigInteger(String.valueOf(MARCIN_31)))).longValue() * (index + 1) % MARCIN_31) +
                     primeNumbers_.get(index) % MARCIN_31) % MARCIN_31 % bitsetSize_;
        }

        private int sieveLimit() {
            return (int)(hashes_ * Math.log(hashes_) + hashes_ * Math.log(Math.log(hashes_)));
        }

        private void setPrimes() {
            int size = 0;
            if (hashes_ <= 5) {
                size = SIEVE_LIMIT;
            } else {
                size = sieveLimit();
            }
            BitSet bitset = new BitSet(size);
            for (int p = 2; p < size; ++p) {
                if (!bitset.get(p)) {
                    if (primeNumbers_.size() == hashes_) {
                        break;
                    }
                    primeNumbers_.add(p);
                    for (int j = p * p; j < size + 1; j += p)
                        bitset.set(j);
                }
            }
        }

        public BloomFilter(int n, double P) {
            bitsetSize_ = (int)Math.round(-(n * log(2, P)) / (Math.log(2)));
            bitset_ = new BitSet(bitsetSize_);
            hashes_ = (int)Math.round(-log(2, P));
            primeNumbers_ = new ArrayList<>();
            setPrimes();
        }

        public int length() {
            return bitsetSize_;
        }

        public int sizeHash() {
            return hashes_;
        }

        public void add(BigInteger key) {
            for (int i = 0; i < hashes_; ++i) {
                bitset_.set(hash(i, key));
            }
        }

        public boolean search(BigInteger key) {
            for (int i = 0; i < hashes_; ++i) {
                if (!bitset_.get(hash(i, key))) {
                    return false;
                }
            }
            return true;
        }

        public void print(OutputStream out) throws IOException {
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            for (int i = 0; i < bitsetSize_; ++i) {
                if (bitset_.get(i)) {
                    writer.write("1");
                } else {
                    writer.write("0");
                }
            }
            writer.flush();
        }
    }

    private static double log(double base, double arg) {
        return Math.log(arg) / Math.log(base);
    }

    public static void main(String[] args) {
        String setRegex = "^set\\s[0-9]*\\s0\\.[0-9][0-9]*$";
        String commandRegex = "^add\\s[0-9][0-9]*$|^search\\s[0-9][0-9]*$|^print$|";
        String buffer = null;
        Scanner input = new Scanner(System.in);
        int n = 0;
        double P = 0;
        while (input.hasNext()) {
            buffer = input.nextLine();
            if (buffer.length() == 0) {
                continue;
            } else if (buffer.matches(setRegex)) {
                String[] splitCommand = buffer.split(" ");
                n = Integer.parseInt(splitCommand[1]);
                P = Double.parseDouble(splitCommand[2]);
                if (n == 0 || Math.round(-log(2, P)) == 0) {
                    System.out.println("error");
                    continue;
                }
                break;
            } else {
                System.out.println("error");
            }
        }

        if (n != 0 && P > 0 && P < 1) {
            BloomFilter filter = new BloomFilter(n, P);
            System.out.println(filter.length() + " " + filter.sizeHash());
            while (input.hasNext()) {
                buffer = input.nextLine();
                if (buffer.length() == 0) {
                    continue;
                } else if (buffer.matches(commandRegex)) {
                    String[] splitCommand = buffer.split(" ");
                    if (splitCommand[0].equals("add")) {
                        filter.add(new BigInteger(splitCommand[1]));
                    } else if (splitCommand[0].equals("search")) {
                        if (filter.search(new BigInteger(splitCommand[1]))) {
                            System.out.println(1);
                        } else {
                            System.out.println(0);
                        }
                    } else if (splitCommand[0].equals("print")) {
                        try {
                            filter.print(System.out);
                            System.out.println();
                        } catch (IOException ex) {
                            System.err.println(ex.getMessage());
                        }
                    } else {
                        System.out.println("error");
                    }
                } else {
                    System.out.println("error");
                }
            }
        }
    }
}