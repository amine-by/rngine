import { Button, StyleSheet, View } from 'react-native';
import { useNavigation } from '@react-navigation/native';
import type { NavigationProp } from '../App';
export default function Menu() {
  const navigation = useNavigation<NavigationProp>();

  return (
    <View style={styles.container}>
      <Button
        title="Playground"
        onPress={() => navigation.navigate('Playground')}
      />
      <Button title="Snake" onPress={() => navigation.navigate('Snake')} />
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
