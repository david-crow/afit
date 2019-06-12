package edu.afit.scpsolver.parse;

import edu.afit.scpsolver.IDFunction;
import edu.afit.scpsolver.ast.Family;
import edu.afit.scpsolver.ast.FamilySet;
import edu.afit.scpsolver.ast.RSet;
import edu.afit.scpsolver.ast.SCP;
import edu.afit.scpsolver.parse.ParseException;
import edu.afit.scpsolver.parse.SCPParserConstants;
import edu.afit.scpsolver.parse.SCPParserTokenManager;
import edu.afit.scpsolver.parse.SimpleCharStream;
import edu.afit.scpsolver.parse.Token;
import java.io.InputStream;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.util.TreeSet;
import java.util.Vector;

public class SCPParser implements SCPParserConstants {
	public SCPParserTokenManager token_source;
	SimpleCharStream jj_input_stream;
	public Token token;
	public Token jj_nt;
	private int jj_ntk;
	private int jj_gen;
	private final int[] jj_la1;
	private static int[] jj_la1_0;
	private Vector jj_expentries;
	private int[] jj_expentry;
	private int jj_kind;

	static {
		jj_la1_0();
	}

	public final SCP SCP() throws ParseException {
		RSet r = this.RSet();
		FamilySet f = this.FamilySet();
		this.jj_consume_token(0);
		return new SCP(r, f);
	}

	public final RSet RSet() throws ParseException {
		TreeSet r = new TreeSet();
		this.jj_consume_token(7);

		while (true) {
			switch (this.jj_ntk == -1 ? this.jj_ntk() : this.jj_ntk) {
				case 6 :
					Integer i = this.IntLiteral();
					r.add(i);
					break;
				default :
					this.jj_la1[0] = this.jj_gen;
					this.jj_consume_token(8);
					return new RSet(r);
			}
		}
	}

	public final FamilySet FamilySet() throws ParseException {
		TreeSet fs = new TreeSet(new IDFunction());
		int id = 1;
		this.jj_consume_token(7);

		while (true) {
			switch (this.jj_ntk == -1 ? this.jj_ntk() : this.jj_ntk) {
				case 9 :
					Family f = this.Family(id);
					fs.add(f);
					++id;
					break;
				default :
					this.jj_la1[1] = this.jj_gen;
					this.jj_consume_token(8);
					return new FamilySet(fs);
			}
		}
	}

	public final Family Family(int id) throws ParseException {
		this.jj_consume_token(9);
		RSet r = this.RSet();
		this.jj_consume_token(10);
		Integer i = this.IntLiteral();
		this.jj_consume_token(11);
		return new Family(r, i, id);
	}

	public final Integer IntLiteral() throws ParseException {
		Token t = this.jj_consume_token(6);
		return Integer.valueOf(Integer.parseInt(t.image));
	}

	private static void jj_la1_0() {
		jj_la1_0 = new int[]{64, 512};
	}

	public SCPParser(InputStream stream) {
		this(stream, (String) null);
	}

	public SCPParser(InputStream stream, String encoding) {
		this.jj_la1 = new int[2];
		this.jj_expentries = new Vector();
		this.jj_kind = -1;

		try {
			this.jj_input_stream = new SimpleCharStream(stream, encoding, 1, 1);
		} catch (UnsupportedEncodingException arg3) {
			throw new RuntimeException(arg3);
		}

		this.token_source = new SCPParserTokenManager(this.jj_input_stream);
		this.token = new Token();
		this.jj_ntk = -1;
		this.jj_gen = 0;

		for (int i = 0; i < 2; ++i) {
			this.jj_la1[i] = -1;
		}

	}

	public void ReInit(InputStream stream) {
		this.ReInit(stream, (String) null);
	}

	public void ReInit(InputStream stream, String encoding) {
		try {
			this.jj_input_stream.ReInit(stream, encoding, 1, 1);
		} catch (UnsupportedEncodingException arg3) {
			throw new RuntimeException(arg3);
		}

		this.token_source.ReInit(this.jj_input_stream);
		this.token = new Token();
		this.jj_ntk = -1;
		this.jj_gen = 0;

		for (int i = 0; i < 2; ++i) {
			this.jj_la1[i] = -1;
		}

	}

	public SCPParser(Reader stream) {
		this.jj_la1 = new int[2];
		this.jj_expentries = new Vector();
		this.jj_kind = -1;
		this.jj_input_stream = new SimpleCharStream(stream, 1, 1);
		this.token_source = new SCPParserTokenManager(this.jj_input_stream);
		this.token = new Token();
		this.jj_ntk = -1;
		this.jj_gen = 0;

		for (int i = 0; i < 2; ++i) {
			this.jj_la1[i] = -1;
		}

	}

	public void ReInit(Reader stream) {
		this.jj_input_stream.ReInit(stream, 1, 1);
		this.token_source.ReInit(this.jj_input_stream);
		this.token = new Token();
		this.jj_ntk = -1;
		this.jj_gen = 0;

		for (int i = 0; i < 2; ++i) {
			this.jj_la1[i] = -1;
		}

	}

	public SCPParser(SCPParserTokenManager tm) {
		this.jj_la1 = new int[2];
		this.jj_expentries = new Vector();
		this.jj_kind = -1;
		this.token_source = tm;
		this.token = new Token();
		this.jj_ntk = -1;
		this.jj_gen = 0;

		for (int i = 0; i < 2; ++i) {
			this.jj_la1[i] = -1;
		}

	}

	public void ReInit(SCPParserTokenManager tm) {
		this.token_source = tm;
		this.token = new Token();
		this.jj_ntk = -1;
		this.jj_gen = 0;

		for (int i = 0; i < 2; ++i) {
			this.jj_la1[i] = -1;
		}

	}

	private final Token jj_consume_token(int kind) throws ParseException {
		Token oldToken = this.token;
		if (this.token.next != null) {
			this.token = this.token.next;
		} else {
			this.token = this.token.next = this.token_source.getNextToken();
		}

		this.jj_ntk = -1;
		if (this.token.kind == kind) {
			++this.jj_gen;
			return this.token;
		} else {
			this.token = oldToken;
			this.jj_kind = kind;
			throw this.generateParseException();
		}
	}

	public final Token getNextToken() {
		if (this.token.next != null) {
			this.token = this.token.next;
		} else {
			this.token = this.token.next = this.token_source.getNextToken();
		}

		this.jj_ntk = -1;
		++this.jj_gen;
		return this.token;
	}

	public final Token getToken(int index) {
		Token t = this.token;

		for (int i = 0; i < index; ++i) {
			if (t.next != null) {
				t = t.next;
			} else {
				t = t.next = this.token_source.getNextToken();
			}
		}

		return t;
	}

	private final int jj_ntk() {
		return (this.jj_nt = this.token.next) == null
				? (this.jj_ntk = (this.token.next = this.token_source.getNextToken()).kind)
				: (this.jj_ntk = this.jj_nt.kind);
	}

	public ParseException generateParseException() {
		this.jj_expentries.removeAllElements();
		boolean[] la1tokens = new boolean[12];

		int exptokseq;
		for (exptokseq = 0; exptokseq < 12; ++exptokseq) {
			la1tokens[exptokseq] = false;
		}

		if (this.jj_kind >= 0) {
			la1tokens[this.jj_kind] = true;
			this.jj_kind = -1;
		}

		int i;
		for (exptokseq = 0; exptokseq < 2; ++exptokseq) {
			if (this.jj_la1[exptokseq] == this.jj_gen) {
				for (i = 0; i < 32; ++i) {
					if ((jj_la1_0[exptokseq] & 1 << i) != 0) {
						la1tokens[i] = true;
					}
				}
			}
		}

		for (exptokseq = 0; exptokseq < 12; ++exptokseq) {
			if (la1tokens[exptokseq]) {
				this.jj_expentry = new int[1];
				this.jj_expentry[0] = exptokseq;
				this.jj_expentries.addElement(this.jj_expentry);
			}
		}

		int[][] arg3 = new int[this.jj_expentries.size()][];

		for (i = 0; i < this.jj_expentries.size(); ++i) {
			arg3[i] = (int[]) ((int[]) this.jj_expentries.elementAt(i));
		}

		return new ParseException(this.token, arg3, tokenImage);
	}

	public final void enable_tracing() {
	}

	public final void disable_tracing() {
	}
}