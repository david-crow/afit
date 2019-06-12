package edu.afit.scpsolver.ast;

import edu.afit.scpsolver.ast.ASTNode;
import edu.afit.scpsolver.ast.ASTVisitor;
import java.util.Iterator;
import java.util.Set;

public class RSet extends ASTNode {
	private Set<Integer> r;

	public RSet(Set<Integer> r) {
		this.r = r;
	}

	public void accept(ASTVisitor v) {
		v.visit(this);
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("{ ");
		Iterator arg2 = this.r.iterator();

		while (arg2.hasNext()) {
			Integer i = (Integer) arg2.next();
			sb.append(i.toString() + " ");
		}

		sb.append("}");
		return sb.toString();
	}

	public Set<Integer> getR() {
		return this.r;
	}
}