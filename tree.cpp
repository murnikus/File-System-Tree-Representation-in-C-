#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace atl {

class TreeNode {
    std::string name;
    bool isDirect;
    std::vector<TreeNode*> child;

    public:

    TreeNode(const std::string& name, bool isDirectory) : name(name), isDirect(isDirectory) {}

    ~TreeNode() {
        for (TreeNode* tmp : child) {
            delete tmp;
        }
    }

    void addChild(TreeNode* tmp) {
        child.push_back(tmp);
    }

    void print(int idx = 0) const {

        for (int i = 0; i < idx; i++) {
            std::cout << "  ";  
        }

        std::cout << (isDirect ? "Dir: " : "File: ") << name <<  std::endl;    

        for (auto& it : child) {
            it->print(idx + 1);
        }
    }

    static TreeNode* buildTree(const std::filesystem::path& path) {

        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("Error: Path does not exist.");
        }

        bool isDirectory = std::filesystem::is_directory(path);

        TreeNode* node = new TreeNode(path.filename().string(), isDirectory);

         if (isDirectory) {
            try {
                for (const auto& entry : std::filesystem::directory_iterator(path)) {
                    TreeNode* childNode = buildTree(entry.path());
                    node->addChild(childNode);
                }
            } catch (const std::filesystem::filesystem_error& e) {
                
                std::cerr << "Warning: Unable to access directory: " << path << "\n";
                std::cerr << e.what() << std::endl;
            }
        }

        return node;
    }

};

} // namespace atl

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << " " << argv[0] << " Ther is no directory" << std::endl;
        return 1;
    }

    std::filesystem::path rootPath = argv[1];

        atl::TreeNode* root = atl::TreeNode::buildTree(rootPath);
        root->print();
        delete root;

    return 0;
}
