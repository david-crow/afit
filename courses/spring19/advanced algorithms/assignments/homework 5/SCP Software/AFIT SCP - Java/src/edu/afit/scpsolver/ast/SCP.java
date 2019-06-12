package edu.afit.scpsolver.ast;

import edu.afit.scpsolver.IDFunction;
import edu.afit.scpsolver.ast.ASTNode;
import edu.afit.scpsolver.ast.ASTVisitor;
import edu.afit.scpsolver.ast.Family;
import edu.afit.scpsolver.ast.FamilySet;
import edu.afit.scpsolver.ast.RSet;
import java.util.Iterator;
import java.util.TreeSet;

public class SCP extends ASTNode {
	private RSet r;
	private FamilySet fs;

	public SCP(RSet r, FamilySet f) {
		this.r = r;
		this.fs = f;
	}

	public void accept(ASTVisitor v) {
		if (v.visit(this)) {
			this.getRSet().accept(v);
			this.getFamilySet().accept(v);
		}

	}

	public RSet getRSet() {
		return this.r;
	}

	public FamilySet getFamilySet() {
		return this.fs;
	}

	public String toString() {
		return this.r.toString() + "\n" + this.fs.toString();
	}

	public SCP clone() {
		TreeSet newR = new TreeSet(this.r.getR());
		RSet rs = new RSet(newR);
		TreeSet families = new TreeSet(new IDFunction());
		Iterator arg4 = this.fs.getFs().iterator();

		while (arg4.hasNext()) {
			Family newFS = (Family) arg4.next();
			families.add(new Family(newFS));
		}

		FamilySet newFS1 = new FamilySet(families);
		return new SCP(rs, newFS1);
	}

	public boolean equals(SCP scp) {
		return this.getRSet().getR().equals(scp.getRSet().getR())
				&& this.fs.getFs().size() == scp.getFamilySet().getFs().size();
	}
}