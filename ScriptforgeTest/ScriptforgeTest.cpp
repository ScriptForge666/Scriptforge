import std;
import Scriptforge;
using namespace std;
using namespace Scriptforge;
int main() {
	try {
		throw Err::Error{ "E0000", "测试错误" };
	}
	catch (const Err::Error& e) {
		cout << e << endl;
	}
	Tree::Tree<int> t1;
	t1.add(t1.root(), 1);
	for (const auto& child : t1.root()->children) {
		cout << child->node << endl;
	}
	return 0;
}
