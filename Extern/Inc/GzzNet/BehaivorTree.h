#pragma once
#include <functional>
#include <vector>

namespace gzz
{
	struct Node
	{
		Node* Up;
		std::vector<Node*> Nodes;
		int Seed;

		Node() = delete;
		Node(int seed)
			:Seed(seed), Up(nullptr)
		{
			Nodes.reserve(20);
		}

		virtual ~Node()
		{
			__int64 size = Nodes.size();
			for (int i = 0; i < size; i++)
			{
				if (Nodes[i])
				{
					delete Nodes[i];
					Nodes[i] = nullptr;
				}
			}
		}

		Node* SearchNode(int seed)
		{
			if (seed == Seed)
			{
				return this;
			}
			for (auto node : Nodes)
			{
				if (node)
				{
					if (node->Seed == seed)
					{
						return node;
					}
					else
					{
						Node* ret = node->SearchNode(seed);
						if (ret != nullptr)
						{
							return ret;
						}
					}
				}
			}
			return nullptr;
		}

		virtual int Invoke()
		{
			for (auto node : Nodes)
			{
				node->Invoke();
			}
			return 1;
		};
	};

	using Behaivor = std::function<int()>;

	template<class C>
	using CMF = int(C::*)();

	struct BehaivorNode : public Node
	{
		using FuncName = wchar_t[20];

		BehaivorNode() : Node(-1), Func(nullptr) {};
		BehaivorNode(int seed) : Node(seed), Func(nullptr) {};
		virtual ~BehaivorNode() = default;

		FuncName funcname = L"";
		Behaivor Func;

	public:
		template<class C>
		void AddFunc(C* this_, CMF<C> function)
		{
			Behaivor _func = [=]() { return(this_->*function)(); };
		}

		virtual int Invoke()
		{
			return Func();
		};
	};

	struct SequenceNode : public BehaivorNode
	{
		virtual int Invoke()
		{
			int ret = 0;
			for (auto node : Nodes)
			{
				ret = node->Invoke();
				if (ret != 1)
				{
					return ret;
				}
			}
			return ret;
		};
	};

	struct SelectNode : public BehaivorNode
	{
		virtual int Invoke()
		{
			int ret = 0;
			for (auto node : Nodes)
			{
				ret = node->Invoke();
				if (ret != 0)
				{
					return ret;
				}
			}
			return ret;
		};
	};

	struct DecoratorNode : public BehaivorNode
	{
		enum DecoratorType { NONE, IF = 205, WHILE, LOOP };
		int Type;

		virtual int Invoke() { return -1; };
	};

	class BehaivorTree
	{
	public:
		BehaivorTree();
		~BehaivorTree();

	private:
		Node* Root;
		std::vector<wchar_t*> FuncNames;
		int BtSeed;

	public:
		bool Insert(BehaivorNode* node, int dst_node);
		bool Erase(Node* node);

		int Run();
	};
}