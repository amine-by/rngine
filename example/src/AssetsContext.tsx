import { createContext, useContext, useState, type ReactNode } from 'react';
import { loadAssets } from 'rngine';
import ASSETS from './assets';

type AssetsType = {
  [K in keyof typeof ASSETS]?: Record<keyof (typeof ASSETS)[K], number>;
};

type GameType = keyof typeof ASSETS;

type AssetsContextType = {
  getAssets: <T extends GameType>(game: T) => AssetsType[T];
  loadAssetsFor: (game: GameType) => Promise<void>;
};

const AssetsContext = createContext<AssetsContextType | null>(null);

export function AssetsProvider({ children }: { children: ReactNode }) {
  const [assets, setAssets] = useState<AssetsType>({});

  const loadAssetsFor = async (game: GameType) => {
    const loadedAssets = await loadAssets(ASSETS[game]);
    setAssets((prev) => {
      return { ...prev, [game]: loadedAssets };
    });
  };

  const getAssets = <T extends GameType>(game: T) => {
    return assets[game];
  };

  return (
    <AssetsContext.Provider value={{ getAssets, loadAssetsFor }}>
      {children}
    </AssetsContext.Provider>
  );
}

export function useAssets() {
  const context = useContext(AssetsContext);

  if (!context) {
    throw new Error('useAssets must be used within a AssetsProvider');
  }

  return context;
}
