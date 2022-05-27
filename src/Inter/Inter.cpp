#include "Inter.h"
/* Scope类的实现 */
/*
Scope::Scope(Scope* n) {
	prev = n;
}
void Scope::put(Token* w, Id* i) {
	table.insert(std::pair<Token, Id>(&w, &i));
}
Id* Scope::get(Token* w) {
	for (Scope sc = this; sc != null; sc = sc.prev) {
		Id* found = (Id)(sc.table.get(w));
		if (found != nullptr)
			return found;
	}
	return nullptr
}
*/
