

const int ALPHA = 0;
const int BETA = 1;

int alphabeta(int a, int b, Tree * tree, int level){
	if(tree.isleaf()){
		return tree.get_value();
	}
	
	int alpha = a;
	int beta = b;
	
	if(level == ALPHA){
	  foreach branch as tree.get_branches(){
		  int value = alphabeta(alpha, beta, branch, BETA);
			alpha = alpha > value ? alpha : value;
		}
		return alpha;
	}
	
	if(level == BETA){
		foreach branch as tree.get_branches(){
		  int value = alphabeta(alpha, beta, branch, ALPHA);
			beta = beta < value ? beta : value;
			if(beta < alpha) break
		}
		return beta;
	}
}