#pragma once

namespace Razor
{
	class Node;
	class Camera;

	class BillboardManager
	{
	public:
		BillboardManager();
		~BillboardManager();

		typedef std::map<unsigned int, std::pair<glm::vec3, std::string>> BillboardMap;

		void addBillboard(unsigned int node_id, const glm::vec3& position, const std::string& icon_name);
		bool removeBillboard(unsigned int node_id);
		void updatePosition(std::shared_ptr<Node> node, Camera* camera, const glm::vec4& viewport);

		inline BillboardMap& getBillboards() { return billboards; }

	private:
		BillboardMap billboards;
	};

}
