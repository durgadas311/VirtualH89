public class V89JNI {
	static {
		System.loadLibrary("V89JNI"); // Load native library at runtime
	}

	private native void startV89(String[] args);

	// Test Driver
	public static void main(String[] args) {
		new V89JNI().startV89(args);
		System.exit(0);
	}

	public void putc(int c) {
		if (c == '\n') System.err.write('!');
		System.err.write(c);
		System.err.flush();
	}

	public void puts(String s) {
		System.err.format("JNI: %s", s);
		System.err.flush();
	}
}
