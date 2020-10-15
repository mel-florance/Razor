#include "rzpch.h"
#include "HuffmanEncoding.h"

namespace Razor
{

	HuffmanEncoding::HuffmanEncoding()
	{
	}

	HuffmanEncoding::~HuffmanEncoding()
	{
	}

	std::string HuffmanEncoding::encode(const std::string& str)
	{
		codes.clear();
		tree = TreeQueue();
		frequencies.clear();

		computeFrequencies(str);

		TreeNode* left, * right, * top;
		std::string out;
		std::map<char, int>::iterator it = frequencies.begin();

		for (; it != frequencies.end(); it++)
			tree.push(new TreeNode(it->first, it->second));
		
		if (tree.size() == 1)
		{
			addCodes(tree.top(), "1");
			printCodes(tree.top(), "1");
		}
		else
		{
			while (tree.size() > 1)
			{
				left = tree.top();
				tree.pop();

				right = tree.top();
				tree.pop();

				top = new TreeNode('$', left->frequency + right->frequency);
				top->left = left;
				top->right = right;

				tree.push(top);
				decode_tree.push(top);
			}

			addCodes(tree.top(), "");
			printCodes(tree.top(), "");
		}
		
		for (auto i : str)
			out += codes[i];

		printStats();
		
		return out;
	}

	std::string HuffmanEncoding::decode(const std::string& str)
	{
		std::string out;
		TreeNode* root = decode_tree.top();
		TreeNode* current = root;

		if (decode_tree.size() == 1)
		{
			for (auto i : str)
				out += root->data;
		}
		else
		{
			printCodes(root, "");

			for (auto c : str)
			{
				if (current != nullptr)
				{
					current = (c == '0') ? current->left : current->right;

					if (current != nullptr)
					{
						if (current->left == nullptr && current->right == nullptr)
						{
							out += current->data;
							current = root;
						}
					}
				}
			}
		}
		
		return out + '\0';
	}

	void HuffmanEncoding::computeFrequencies(const std::string& str)
	{
		for (int i = 0; i < str.length(); i ++)
			frequencies[str[i]]++;
	}

	void HuffmanEncoding::addCodes(TreeNode* root, const std::string& str)
	{
		if (root == nullptr) return;

		if (root->data != '$')
			codes[root->data] = str;

		addCodes(root->left, str + "0");
		addCodes(root->right, str + "1");
	}

	void HuffmanEncoding::printCodes(TreeNode* root, const std::string& str)
	{
		if (root == nullptr) return;

		if (root->data != '$')
			std::cout << root->data << " : " << str << " : " << frequencies[root->data] << std::endl;

		printCodes(root->left, str + "0");
		printCodes(root->right, str + "1");
	}

	void HuffmanEncoding::printStats()
	{
		int i = (int)getInputSize();
		int o = (int)getOutputSize();

		std::cout << "Input size: " << Utils::bytesToSize(i) << std::endl;
		std::cout << "Output size: " << Utils::bytesToSize(o) << std::endl;
		std::cout << "Compression gain: " << ((1.0f - ((float)o / i)) * 100.0f) << " %" << std::endl;
	}

	unsigned int HuffmanEncoding::getInputSize()
	{
		unsigned int sum = 0;

		for (auto c = codes.begin(); c != codes.end(); c++)
			sum += frequencies[c->first] * 8;

		return sum;
	}

	unsigned int HuffmanEncoding::getOutputSize()
	{
		unsigned int sum = 0;

		for (auto c = codes.begin(); c != codes.end(); c++)
			sum += frequencies[c->first] * (unsigned int)c->second.length();

		return sum;
	}

}
