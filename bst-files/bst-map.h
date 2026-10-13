#ifndef BSTMAP_H
#define BSTMAP_H
#include <iostream>
namespace CS280 {

    template< typename KEY_TYPE, typename VALUE_TYPE >
    class BSTmap {
		public:
			class Node {
				public:
					Node(KEY_TYPE k, VALUE_TYPE val, Node* p, int h, int b, Node* l, Node* r)
						: key(k), value(val), height(h), balance(b), parent(p), left(l), right(r) {}

					Node(const Node&)               = delete;
					Node* operator=(const Node&)    = delete;
					
					KEY_TYPE const& Key() const {return key;}   // return a const reference
					VALUE_TYPE& Value() { return value; }       // return a reference
					
					// minimum - follow left as far as possible
					Node* first() {
						if (this->left == nullptr) {
							return this;
						}
						else {
							return this->left->first();
						}
					} // minimum - follow left as far as possible
					
					// maximum - follow right as far as possible
					Node* last() {
						if (this->right == nullptr) {
							return this;
						}
						else {
							return this->right->last();
						}
					} // maximum - follow right as far as possible
					
					// successor
					Node* increment() {
						if (this->right != nullptr) {
							return right->first();
						}
						
						else {
							Node* prev = this;
							Node* node = parent;
							while (node != nullptr && prev == node->right) {
								prev = node;
								node = node->parent;
							}
							return node;
						}
					} // successor
					
					// predecessor
					Node* decrement() {
						if (left != nullptr) {
							return left->last();
						}
						Node* prev = this;
						Node* node = parent;
						while (node != nullptr && prev == node->left) {
							prev = node;
							node = node->parent;
						}
						return node;
					} // predecessor
					void   print(std::ostream& os) const {
						os << "(" << key << "," << value << ")";
					}
				private:
                    KEY_TYPE    key;
					VALUE_TYPE  value;
					int         height,balance; // optional
					Node        *parent;
					Node        *left;
					Node        *right;

					friend class BSTmap;
			};
        private:
			struct BSTmap_iterator {
				private:
					Node* p_node;
				public:
					BSTmap_iterator(Node* p = nullptr) : p_node(p) {}
					BSTmap_iterator(const BSTmap_iterator&) = default;
					BSTmap_iterator& operator=(const BSTmap_iterator& rhs) {
						p_node = rhs.p_node;
						return *this;
					}
					BSTmap_iterator& operator++() {
						if (p_node->right) {
							p_node = p_node->right;
							while (p_node->left) {
								p_node = p_node->left;
							}
						}
						else {
							Node* parent = p_node->parent;
							while (parent && parent->right == p_node) {
								parent = parent->parent;
								p_node = p_node->parent;
							}
							p_node = p_node->parent;
						}
						return *this;
					}
					BSTmap_iterator operator++(int){
						BSTmap_iterator temp = *this;
						++(*this);
						return temp;
					}
					Node& operator*() { return *this->p_node; }
					Node* operator->() { return this->p_node; }
					bool operator!=(const BSTmap_iterator& rhs) { return p_node != rhs.p_node; }
                    bool operator==(const BSTmap_iterator& rhs){return p_node == rhs.p_node;}
					friend class BSTmap;
			};
			struct BSTmap_iterator_const {
				private:
					Node* p_node;
				public:
					BSTmap_iterator_const(Node* p = nullptr) :p_node(p) {}					
					BSTmap_iterator_const(const BSTmap_iterator_const&) = default;
					BSTmap_iterator_const& operator=(const BSTmap_iterator_const& rhs) {
						p_node = rhs.p_node;
						return *this;
					}
					BSTmap_iterator_const& operator++(){
						if (p_node->right) {
							p_node = p_node->right;
							while (p_node->left) {
								p_node = p_node->left;
							}
						}
						else {
							Node* parent = p_node->parent;
							while (parent && parent->right == p_node) {
								parent = parent->parent;
								p_node = p_node->parent;
							}
							p_node = p_node->parent;
						}
						return *this;
					}
					BSTmap_iterator_const operator++(int){
						BSTmap_iterator_const temp = *this;
						++(*this);
						return temp;
					}
                    Node const& operator*() { return *this->p_node; }
					Node const* operator->() { return this->p_node; }
					bool operator!=(const BSTmap_iterator_const& rhs) { return p_node != rhs.p_node; }
					bool operator==(const BSTmap_iterator_const& rhs) { return p_node == rhs.p_node; }
					friend class BSTmap;
			};
            // BSTmap implementation
			Node* pRoot = nullptr;
            unsigned int size_ = 0;
			// end iterators are same for all BSTmaps, thus static
			// make BSTmap_iterator a friend
			// to allow BSTmap_iterator to access end iterators 
			static BSTmap_iterator end_it;
			static BSTmap_iterator_const const_end_it;
		public:
			//BIG FOUR
			BSTmap() : pRoot(nullptr), size_(0) {}
			BSTmap(const BSTmap& rhs);
			BSTmap& operator=(const BSTmap& rhs) {
				BSTmap bst(rhs);
				std::swap(pRoot, bst.pRoot);
				std::swap(size_, bst.size_);
				return *this;
			}
			virtual ~BSTmap() {
				destroy(pRoot);
				pRoot = nullptr;
				size_ = 0;
			}

			unsigned int size() { return size_; }

			//value setter and getter
			VALUE_TYPE& operator[](KEY_TYPE const& key){
				Node* node = insert(key);
				return node->Value();
			}
			//next method doesn't make sense
			//because operator[] inserts a non-existing element
			//which is not allowed on const maps
			//VALUE_TYPE operator[](int key) const;

			//standard names for iterator types
			typedef BSTmap_iterator       iterator;
			typedef BSTmap_iterator_const const_iterator;

			//BSTmap methods dealing with non-const iterator 
			BSTmap_iterator begin() {
				if (pRoot) 
					return BSTmap_iterator(pRoot->first());
				else       
					return end_it;
			}
			BSTmap_iterator end() {
				return end_it;
			}
			BSTmap_iterator find(KEY_TYPE const& key) {
				Node* cur = this->pRoot;

				while (cur) {
					if (cur->key == key)
						return iterator(cur);

					if (cur->key < key)
						cur = cur->right;

					else cur = cur->left;
				}
				return this->end();
			}
			void erase(BSTmap_iterator it) {
				//destroy(it.p_node);
				Node* node = it.p_node;
if (node == nullptr) {
	return;
}

Node* pre = (node->left == nullptr || node->right == nullptr) ? node : node->increment();
Node* temp = (pre->left != nullptr) ? pre->left : pre->right;

if (temp != nullptr) {
	temp->parent = pre->parent;
}

if (pre->parent == nullptr) {
	pRoot = temp;
}
else if (pre == pre->parent->left) {
	pre->parent->left = temp;
}
else {
	pre->parent->right = temp;
}

if (pre != node) {
	const_cast<KEY_TYPE&>(node->key) = pre->key;
	node->value = pre->value;
}

delete pre;
size_--;
			}

			//BSTmap methods dealing with const iterator 
			BSTmap_iterator_const begin() const {
				if (pRoot == nullptr) {
					return end();
				}
				return const_iterator(pRoot->first());
			}
			BSTmap_iterator_const end() const {
				return const_end_it;
			}
			BSTmap_iterator_const find(KEY_TYPE const& key) const {
				Node* cur = this->pRoot;

				while (cur) {
					if (cur->key == key)
						return const_iterator(cur);

					if (cur->key < key)
						cur = cur->right;

					else cur = cur->left;
				}
				return this->end();
			}
			//do not need this one (why)
			//BSTmap_iterator_const erase(BSTmap_iterator& it) const;

			void print(std::ostream& os, bool print_value = false) const;
			//bool sanityCheck();

			//inner class (BSTmap_iterator) doesn't have any special priveleges
			//in accessing private data/methods of the outer class (BSTmap)
			//so need friendship to allow BSTmap_iterator to access private "BSTmap::end_it"
			//BTW - same is true for outer class accessing inner class private data
			friend class BSTmap_iterator;
			friend class BSTmap_iterator_const;

			char getedgesymbol(const Node* node) const;
			int getdepth(const Node* node)const;
		private:
            // ...
			Node* insert(KEY_TYPE const& key) {
				Node* parent = nullptr;
				Node* cur = pRoot;

				while (cur) {
					parent = cur;

					if (key < cur->key)
						cur = cur->left;
					else if (key > cur->key)
						cur = cur->right;
					else
						return cur;
				}

				Node* newNode = new Node(key, VALUE_TYPE(), parent, 0, 0, nullptr, nullptr);

				if (!parent)
					pRoot = newNode;
				else if (key < parent->key)
					parent->left = newNode;
				else
					parent->right = newNode;

				size_++;

				return newNode;
			}
			void destroy(Node* node) {

				if (node) {
					if (node->left)
						destroy(node->left);
					if (node->right)
						destroy(node->right);
					delete node;
					size_--;
				}
			}
			Node* copy(Node* source, Node* parent)
			{
				if (source == nullptr) return nullptr;

				Node* node = new Node(source->key, source->value, parent, source->height, source->balance, nullptr, nullptr);

				node->left = copy(source->left, node);
				node->right = copy(source->right, node);

				return node;
			}
	};

	//notice that it doesn't need to be friend
    template< typename KEY_TYPE, typename VALUE_TYPE >
	std::ostream& operator<<(std::ostream& os, BSTmap<KEY_TYPE, VALUE_TYPE> const& map);
}

#include "bst-map.cpp"
#endif
