import codecs

with codecs.open('Source/SceneGame.cpp', 'r', 'utf-8-sig') as f:
    text = f.read()

old_update = '''void SceneGame::Update()
{
	if (!Master::is_cutscene_playing_) {
		Scene::Update();
	}
	if (game_manager_)
	{
		game_manager_->Update();
	}
}'''

new_update = '''void SceneGame::Update()
{
	if (Master::hit_stop_timer_ > 0)
	{
		Master::hit_stop_timer_--;
		return;
	}

	if (!Master::is_cutscene_playing_) {
		Scene::Update();
	}
	if (game_manager_)
	{
		game_manager_->Update();
	}
}'''

text = text.replace(old_update, new_update)

with codecs.open('Source/SceneGame.cpp', 'w', 'utf-8-sig') as f:
    f.write(text)
