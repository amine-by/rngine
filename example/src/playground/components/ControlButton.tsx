import { StyleSheet, Text, View, type ViewProps } from 'react-native';
import { GestureDetector } from 'react-native-gesture-handler';

type ControlButtonProps = ViewProps & {
  gesture: any;
};

function ControlButton({
  style,
  children,
  gesture,
  ...props
}: ControlButtonProps) {
  return (
    <GestureDetector gesture={gesture}>
      <View style={[styles.controlButton, style]} {...props}>
        <Text style={styles.buttonText} children={children} />
      </View>
    </GestureDetector>
  );
}

const styles = StyleSheet.create({
  controlButton: {
    width: 60,
    height: 60,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#009bff',
  },
  buttonText: {
    color: '#fff',
  },
});

export { ControlButton };
