package edu.afit.scpsolver;

import edu.afit.scpsolver.P;
import edu.afit.scpsolver.ast.Family;

public class StackPair {
	public P p;
	public Family f;

	public StackPair(P p, Family f) {
		this.p = p;
		this.f = f;
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("|[" + this.p.getPrimaryElement() + "], " + this.f + "|");
		return sb.toString();
	}
}