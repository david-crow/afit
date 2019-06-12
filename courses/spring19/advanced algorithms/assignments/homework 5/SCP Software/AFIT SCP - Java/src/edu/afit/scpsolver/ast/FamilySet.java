package edu.afit.scpsolver.ast;

import edu.afit.scpsolver.ast.ASTNode;
import edu.afit.scpsolver.ast.ASTVisitor;
import edu.afit.scpsolver.ast.Family;
import java.util.Iterator;
import java.util.Set;

public class FamilySet extends ASTNode {
	private Set<Family> fs;

	public FamilySet(Set<Family> fs) {
		this.fs = fs;
	}

	public void accept(ASTVisitor v) {
		v.visit(this);
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("{\n");
		Iterator arg2 = this.fs.iterator();

		while (arg2.hasNext()) {
			Family f = (Family) arg2.next();
			sb.append("  " + f.toString() + "\n");
		}

		sb.append("}");
		return sb.toString();
	}

	public Set<Family> getFs() {
		return this.fs;
	}
}