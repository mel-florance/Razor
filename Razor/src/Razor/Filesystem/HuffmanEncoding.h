#pragma 

namespace Razor
{

	class HuffmanEncoding
	{
	public:
		HuffmanEncoding();
		~HuffmanEncoding();

		struct TreeNode
		{
			TreeNode(char data, int frequency) :
				left(nullptr),
				right(nullptr),
				data(data),
				frequency(frequency) {}
			
			~TreeNode() {
				delete left;
				delete right;
			}

			char data;
			int frequency;
			TreeNode* left;
			TreeNode* right;
		};

		struct TreeNodeCompare
		{
			bool operator()(TreeNode* left, TreeNode* right)
			{
				return left->frequency > right->frequency;
			}
		};

		typedef std::priority_queue<TreeNode*, std::vector<TreeNode*>, TreeNodeCompare> TreeQueue;

		std::string encode(const std::string& str);
		std::string decode(const std::string& str);

		void computeFrequencies(const std::string& str);
		void addCodes(TreeNode* root, const std::string& str);
		void printCodes(TreeNode* root, const std::string& str);
		void printStats();
		unsigned int getInputSize();
		unsigned int getOutputSize();

	private:
		TreeQueue tree;
		TreeQueue decode_tree;
		std::map<char, std::string> codes;
		std::map<char, int> frequencies;
	};

}
