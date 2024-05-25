#pragma once

#include <vector>
#include <string>

class ResourceManager
{
private:

	static	std::vector<int>			handles;		// ハンドル(テクスチャもサウンドもこれ一つ)
	static	std::vector<std::string>	identifiers;	// 一意な名前(識別子)

	static	int		_GetIndex(const char* _name);

public:

	/// <summary>
	/// リソースを登録する (リソースの読み込みも実行される)
	/// </summary>
	/// <param name="_name">一意な名前(識別子)</param>
	/// <param name="_path">リソースの相対パス</param>
	/// <param name="_isTexture">リソースがテクスチャかどうか</param>
	static	void	Regist(const char* _name, const char* _path, bool _isTexture = true);

	/// <summary>
	/// 取得したハンドルを検索する
	/// </summary>
	/// <param name="_name">登録した一意な名前(識別子)</param>
	/// <returns>成功: ハンドル / 失敗: -1</returns>
	static	int		Handle(const char* _name);

	/// <summary>
	/// ロードしたリソースの専有ロックを解除する ※テクスチャのみ対応
	/// </summary>
	/// <param name="_name">登録した一意な名前(識別子)</param>
	static	void	Unload(const char* _name);
};
