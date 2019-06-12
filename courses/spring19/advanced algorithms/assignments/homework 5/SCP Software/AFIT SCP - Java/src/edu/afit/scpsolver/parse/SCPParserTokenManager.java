package edu.afit.scpsolver.parse;

import edu.afit.scpsolver.parse.SCPParserConstants;
import edu.afit.scpsolver.parse.SimpleCharStream;
import edu.afit.scpsolver.parse.Token;
import edu.afit.scpsolver.parse.TokenMgrError;
import java.io.IOException;
import java.io.PrintStream;

public class SCPParserTokenManager implements SCPParserConstants {
	public PrintStream debugStream;
	static final int[] jjnextStates = new int[0];
	public static final String[] jjstrLiteralImages = new String[]{"", null, null, null, null, null, null, "{", "}",
			"(", ",", ")"};
	public static final String[] lexStateNames = new String[]{"DEFAULT"};
	static final long[] jjtoToken = new long[]{4033L};
	static final long[] jjtoSkip = new long[]{62L};
	protected SimpleCharStream input_stream;
	private final int[] jjrounds;
	private final int[] jjstateSet;
	protected char curChar;
	int curLexState;
	int defaultLexState;
	int jjnewStateCnt;
	int jjround;
	int jjmatchedPos;
	int jjmatchedKind;

	public void setDebugStream(PrintStream ds) {
		this.debugStream = ds;
	}

	private final int jjStopStringLiteralDfa_0(int pos, long active0) {
		return -1;
	}

	private final int jjStartNfa_0(int pos, long active0) {
		return this.jjMoveNfa_0(this.jjStopStringLiteralDfa_0(pos, active0), pos + 1);
	}

	private final int jjStopAtPos(int pos, int kind) {
		this.jjmatchedKind = kind;
		this.jjmatchedPos = pos;
		return pos + 1;
	}

	private final int jjStartNfaWithStates_0(int pos, int kind, int state) {
		this.jjmatchedKind = kind;
		this.jjmatchedPos = pos;

		try {
			this.curChar = this.input_stream.readChar();
		} catch (IOException arg4) {
			return pos + 1;
		}

		return this.jjMoveNfa_0(state, pos + 1);
	}

	private final int jjMoveStringLiteralDfa0_0() {
		switch (this.curChar) {
			case '(' :
				return this.jjStopAtPos(0, 9);
			case ')' :
				return this.jjStopAtPos(0, 11);
			case ',' :
				return this.jjStopAtPos(0, 10);
			case '{' :
				return this.jjStopAtPos(0, 7);
			case '}' :
				return this.jjStopAtPos(0, 8);
			default :
				return this.jjMoveNfa_0(0, 0);
		}
	}

	private final void jjCheckNAdd(int state) {
		if (this.jjrounds[state] != this.jjround) {
			this.jjstateSet[this.jjnewStateCnt++] = state;
			this.jjrounds[state] = this.jjround;
		}

	}

	private final void jjAddStates(int start, int end) {
		do {
			this.jjstateSet[this.jjnewStateCnt++] = jjnextStates[start];
		} while (start++ != end);

	}

	private final void jjCheckNAddTwoStates(int state1, int state2) {
		this.jjCheckNAdd(state1);
		this.jjCheckNAdd(state2);
	}

	private final void jjCheckNAddStates(int start, int end) {
		do {
			this.jjCheckNAdd(jjnextStates[start]);
		} while (start++ != end);

	}

	private final void jjCheckNAddStates(int start) {
		this.jjCheckNAdd(jjnextStates[start]);
		this.jjCheckNAdd(jjnextStates[start + 1]);
	}

	private final int jjMoveNfa_0(int startState, int curPos) {
		int startsAt = 0;
		this.jjnewStateCnt = 1;
		int i = 1;
		this.jjstateSet[0] = startState;
		int kind = Integer.MAX_VALUE;

		while (true) {
			if (++this.jjround == Integer.MAX_VALUE) {
				this.ReInitRounds();
			}

			long arg11;
			if (this.curChar < 64) {
				arg11 = 1L << this.curChar;

				do {
					--i;
					switch (this.jjstateSet[i]) {
						case 0 :
							if ((287948901175001088L & arg11) != 0L) {
								kind = 6;
								this.jjstateSet[this.jjnewStateCnt++] = 0;
							}
					}
				} while (i != startsAt);
			} else {
				int arg9999;
				if (this.curChar < 128) {
					arg11 = 1L << (this.curChar & 63);

					do {
						--i;
						arg9999 = this.jjstateSet[i];
					} while (i != startsAt);
				} else {
					int e = (this.curChar & 255) >> 6;
					long l2 = 1L << (this.curChar & 63);

					do {
						--i;
						arg9999 = this.jjstateSet[i];
					} while (i != startsAt);
				}
			}

			if (kind != Integer.MAX_VALUE) {
				this.jjmatchedKind = kind;
				this.jjmatchedPos = curPos;
				kind = Integer.MAX_VALUE;
			}

			++curPos;
			if ((i = this.jjnewStateCnt) == (startsAt = 1 - (this.jjnewStateCnt = startsAt))) {
				return curPos;
			}

			try {
				this.curChar = this.input_stream.readChar();
			} catch (IOException arg10) {
				return curPos;
			}
		}
	}

	public SCPParserTokenManager(SimpleCharStream stream) {
		this.debugStream = System.out;
		this.jjrounds = new int[1];
		this.jjstateSet = new int[2];
		this.curLexState = 0;
		this.defaultLexState = 0;
		this.input_stream = stream;
	}

	public SCPParserTokenManager(SimpleCharStream stream, int lexState) {
		this(stream);
		this.SwitchTo(lexState);
	}

	public void ReInit(SimpleCharStream stream) {
		this.jjmatchedPos = this.jjnewStateCnt = 0;
		this.curLexState = this.defaultLexState;
		this.input_stream = stream;
		this.ReInitRounds();
	}

	private final void ReInitRounds() {
		this.jjround = -2147483647;

		for (int i = 1; i-- > 0; this.jjrounds[i] = Integer.MIN_VALUE) {
			;
		}

	}

	public void ReInit(SimpleCharStream stream, int lexState) {
		this.ReInit(stream);
		this.SwitchTo(lexState);
	}

	public void SwitchTo(int lexState) {
		if (lexState < 1 && lexState >= 0) {
			this.curLexState = lexState;
		} else {
			throw new TokenMgrError("Error: Ignoring invalid lexical state : " + lexState + ". State unchanged.", 2);
		}
	}

	protected Token jjFillToken() {
		Token t = Token.newToken(this.jjmatchedKind);
		t.kind = this.jjmatchedKind;
		String im = jjstrLiteralImages[this.jjmatchedKind];
		t.image = im == null ? this.input_stream.GetImage() : im;
		t.beginLine = this.input_stream.getBeginLine();
		t.beginColumn = this.input_stream.getBeginColumn();
		t.endLine = this.input_stream.getEndLine();
		t.endColumn = this.input_stream.getEndColumn();
		return t;
	}

	public Token getNextToken() {
		Object specialToken = null;
		boolean curPos = false;

		while (true) {
			Token matchedToken;
			try {
				this.curChar = this.input_stream.BeginToken();
			} catch (IOException arg9) {
				this.jjmatchedKind = 0;
				matchedToken = this.jjFillToken();
				return matchedToken;
			}

			try {
				this.input_stream.backup(0);

				while (this.curChar <= 32 && (4294981120L & 1L << this.curChar) != 0L) {
					this.curChar = this.input_stream.BeginToken();
				}
			} catch (IOException arg11) {
				continue;
			}

			this.jjmatchedKind = Integer.MAX_VALUE;
			this.jjmatchedPos = 0;
			int arg12 = this.jjMoveStringLiteralDfa0_0();
			if (this.jjmatchedKind == Integer.MAX_VALUE) {
				int error_line = this.input_stream.getEndLine();
				int error_column = this.input_stream.getEndColumn();
				String error_after = null;
				boolean EOFSeen = false;

				try {
					this.input_stream.readChar();
					this.input_stream.backup(1);
				} catch (IOException arg10) {
					EOFSeen = true;
					error_after = arg12 <= 1 ? "" : this.input_stream.GetImage();
					if (this.curChar != 10 && this.curChar != 13) {
						++error_column;
					} else {
						++error_line;
						error_column = 0;
					}
				}

				if (!EOFSeen) {
					this.input_stream.backup(1);
					error_after = arg12 <= 1 ? "" : this.input_stream.GetImage();
				}

				throw new TokenMgrError(EOFSeen, this.curLexState, error_line, error_column, error_after, this.curChar,
						0);
			}

			if (this.jjmatchedPos + 1 < arg12) {
				this.input_stream.backup(arg12 - this.jjmatchedPos - 1);
			}

			if ((jjtoToken[this.jjmatchedKind >> 6] & 1L << (this.jjmatchedKind & 63)) != 0L) {
				matchedToken = this.jjFillToken();
				return matchedToken;
			}
		}
	}
}