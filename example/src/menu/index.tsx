import { Button, StyleSheet, View } from 'react-native';
import { useNavigation } from '@react-navigation/native';
import type { NavigationProp } from '../App';
import { useAssets } from '../AssetsContext';
export default function Menu() {
  const { loadAssetsFor } = useAssets();
  const navigation = useNavigation<NavigationProp>();

  return (
    <View style={styles.container}>
      <Button
        title="Playground"
        onPress={async () => {
          await loadAssetsFor('Playground');
          navigation.navigate('Playground');
        }}
      />
      <Button
        title="Snake"
        onPress={async () => {
          await loadAssetsFor('Snake');
          navigation.navigate('Snake');
        }}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    gap: 16,
  },
});
