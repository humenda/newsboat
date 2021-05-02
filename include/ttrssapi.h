#ifndef NEWSBOAT_TTRSSAPI_H_
#define NEWSBOAT_TTRSSAPI_H_

#include "3rd-party/json.hpp"

#include "cache.h"
#include "remoteapi.h"
#include "utf8string.h"

namespace rsspp {
class Feed;
}

namespace newsboat {

class TtRssApi : public RemoteApi {
public:
	explicit TtRssApi(ConfigContainer* c);
	~TtRssApi() override;
	bool authenticate() override;
	virtual nlohmann::json run_op(const std::string& op,
		const std::map<std::string, std::string>& args,
		bool try_login = true,
		CURL* cached_handle = nullptr);
	std::vector<TaggedFeedUrl> get_subscribed_urls() override;
	void add_custom_headers(curl_slist** custom_headers) override;
	bool mark_all_read(const std::string& feedurl) override;
	bool mark_article_read(const std::string& guid, bool read) override;
	bool update_article_flags(const std::string& oldflags,
		const std::string& newflags,
		const std::string& guid) override;
	rsspp::Feed fetch_feed(const std::string& id, CURL* cached_handle);
	bool update_article(const std::string& guid, int mode, int field);

private:
	void fetch_feeds_per_category(const nlohmann::json& cat,
		std::vector<TaggedFeedUrl>& feeds);
	bool star_article(const std::string& guid, bool star);
	bool publish_article(const std::string& guid, bool publish);
	TaggedFeedUrl feed_from_json(const nlohmann::json& jfeed,
		const std::vector<std::string>& tags);
	int parse_category_id(const nlohmann::json& jcatid);
	unsigned int query_api_level();
	std::string url_to_id(const std::string& url);
	std::string retrieve_sid();

	Utf8String sid;
	Utf8String auth_info;
	bool single;
	std::mutex auth_lock;
	int api_level = -1;
};

} // namespace newsboat

#endif /* NEWSBOAT_TTRSSAPI_H_ */
